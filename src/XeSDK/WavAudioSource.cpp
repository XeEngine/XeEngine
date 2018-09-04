#include "pch.h"
#include <XeSDK/XeMemory.h>
#include <XeSDK/IIoStream.h>
#include <XeSDK/XeSound.h>
#include <cstdint>
#include "WavAudioSource.h"

#define LPREFIX "WavAudioSource: "

static_assert(sizeof(RIFFChunk) == 8, "structure size mismatch");
static_assert(sizeof(RIFFChunkHeader) == 12, "structure size mismatch");
static_assert(sizeof(DLSLoop) == 16, "structure size mismatch");
static_assert(sizeof(RIFFDLSSample) == 20, "structure size mismatch");
static_assert(sizeof(MIDILoop) == 24, "structure size mismatch");
static_assert(sizeof(RIFFMIDISample) == 36, "structure size mismatch");


namespace Xe { namespace Sound {
	WavAudioSource::WavAudioSource() :
		m_pStream(nullptr),
		m_DataPosition(0),
		m_DataLength(0),
		m_SampleSize(0),
		m_SamplesPerSecond(0),
		m_ChannelsCount(0),
		m_BitFormat(BitFormat_Unspecified)
	{

	}

	WavAudioSource::~WavAudioSource()
	{

	}

	bool WavAudioSource::Initialize(Xe::IO::IStream* pStream)
	{
		bool r;
		ASSERT(!!pStream);

		if (pStream->GetLength() - pStream->GetPosition() < (sizeof(RIFFChunk) * 2 + sizeof(uint32_t) + sizeof(WAVEFORMAT)))
		{
			LOGE(LPREFIX"The specified stream is too small to be a valid WAV file.");
			return false;
		}

		if (!pStream->CanRead())
		{
			LOGE(LPREFIX"Specified stream %p does not support reading.", pStream);
			return false;
		}

		RIFFChunkHeader chunkHeader;
		ASSERT(r = pStream->Read(&chunkHeader, 0, sizeof(RIFFChunkHeader)) == sizeof(RIFFChunkHeader));
		if (!r)
		{
			LOGE(LPREFIX"Unable to read WAV header from stream %p.", pStream);
			return false;
		}

		ASSERT(r = chunkHeader.tag == FOURCC_RIFF_TAG);
		if (!r)
		{
			LOGE(LPREFIX"Invalid header for WAV file from stream %p.", pStream);
			return false;
		}

		ASSERT(r = chunkHeader.size >= pStream->GetLength() - pStream->GetPosition() + 4);
		if (!r)
		{
			LOGE(LPREFIX"Invalid size for WAV file from stream %p.", pStream);
			return false;
		}

		ASSERT(r = chunkHeader.riff == FOURCC_WAVE_FILE_TAG || chunkHeader.riff == FOURCC_XWMA_FILE_TAG);
		if (!r)
		{
			LOGE(LPREFIX"Unsupported CHID %08X for WAV file from stream %p.", chunkHeader.riff, pStream);
			return false;
		}

		uint32_t remainingBytes = chunkHeader.size - 4;
		while (remainingBytes >= sizeof(RIFFChunk))
		{
			RIFFChunk chunk;
			ASSERT(r = pStream->Read(&chunk, 0, sizeof(RIFFChunk)) == sizeof(RIFFChunk));
			if (!r)
			{
				LOGE(LPREFIX"Unexpected end of stream %p.", chunkHeader.riff, pStream);
				return false;
			}

			ASSERT(r = pStream->GetLength() - pStream->GetPosition() >= chunk.size);
			if (!r)
			{
				LOGE(LPREFIX"Unexpected end of stream %p.", chunkHeader.riff, pStream);
				return false;
			}

			switch (chunk.tag)
			{
			case FOURCC_FORMAT_TAG:
				ASSERT(r = ChunkFormat(pStream, chunk.size));
				break;
			case FOURCC_DATA_TAG:
				ASSERT(r = ChunkData(pStream, chunk.size));
				break;
			case FOURCC_DLS_SAMPLE:
				ASSERT(r = ChunkDlsSample(pStream, chunk.size));
				break;
			case FOURCC_MIDI_SAMPLE:
				ASSERT(r = ChunkMidiSample(pStream, chunk.size));
				break;
			//case FOURCC_XWMA_DPDS:
			//	ASSERT(r = ChunkXwmaDpds(pStream, chunk.size));
			//	break;
			//case FOURCC_XMA_SEEK:
			//	ASSERT(r = ChunkXmaSeek(pStream, chunk.size));
			//	break;
			default:
				LOGW(LPREFIX"Chunk %08X not supported.", chunk.tag);
				ASSERT(r = pStream->SetPosition(chunk.size, IO::Seek_Cur) == Error::OK);
				break;
			}

			if (!r)
			{
				LOGE(LPREFIX"Unexpected chunk error.");
				return false;
			}

			remainingBytes -= chunk.size + sizeof(RIFFChunk);
		}

		return true;
	}

	bool WavAudioSource::ChunkFormat(Xe::IO::IStream* pStream, uint32_t size)
	{
		bool r;
		ASSERT(r = size >= sizeof(PCMWAVEFORMAT));
		if (!r)
		{
			LOGE(LPREFIX"The size of 'fmt ' chunk is %i, but should be at least %i bytes.", size, sizeof(PCMWAVEFORMAT));
			return false;
		}

		void* fmt = Xe::Memory::Alloc(size);
		ASSERT(pStream->Read(fmt, 0, size) == size);

		WAVEFORMAT* wf = (WAVEFORMAT*)fmt;

		m_ChannelsCount = wf->nChannels;
		m_SamplesPerSecond = wf->nSamplesPerSec;
		m_BitFormat = BitFormat_Unspecified;

		switch (wf->wFormatTag)
		{
		case WAVE_FORMAT_PCM:
			ASSERT(r = FormatPcm(*(PCMWAVEFORMAT*)fmt));
			break;
		case WAVE_FORMAT_ADPCM:
			ASSERT(r = FormatAdpcm(*(WAVEFORMATEX*)fmt));
			break;
		case WAVE_FORMAT_IEEE_FLOAT:
			ASSERT(r = FormatIeeeFloat(*(WAVEFORMAT*)fmt));
			break;
		case WAVE_FORMAT_WMAUDIO2:
			ASSERT(r = FormatWmaAudio2(*(WAVEFORMATEX*)fmt));
			break;
		case WAVE_FORMAT_WMAUDIO3:
			ASSERT(r = FormatWmaAudio3(*(WAVEFORMATEX*)fmt));
			break;
#if _XBOX_ONE
		case WAVE_FORMAT_XMA2:
			ASSERT(r = FormatXma2(*(XMA2WAVEFORMATEX*)fmt));
			break;
#endif
		case WAVE_FORMAT_EXTENSIBLE:
			ASSERT(r = FormatExtendible(*(WAVEFORMATEXTENSIBLE*)fmt));
			break;
		default:
			LOGE(LPREFIX"The format %04X is not supported.", wf->wFormatTag);
			break;
		}

		if (!r)
		{
			LOGE(LPREFIX"Unexpected format reading error.");
			return false;
		}

		m_format.NumberOfChannels = m_ChannelsCount;
		m_format.SampleRate = m_SamplesPerSecond;
		m_format.SampleLength = m_ChannelsCount * sizeof(float);

		Xe::Memory::Free(fmt);
		return true;
	}

	bool WavAudioSource::ChunkData(Xe::IO::IStream* pStream, uint32_t size)
	{
		if (!m_pStream)
		{
			m_pStream = pStream;
			m_pStream->AddRef();
		}

		uint32_t bps = GetBitsPerSample(m_BitFormat);
		m_DataPosition = pStream->GetPosition();
		m_DataLength = size;
		m_posEnd = size / (m_ChannelsCount * bps / 8);

		pStream->SetPosition(size, IO::Seek_Cur);
		return true;
	}

	bool WavAudioSource::ChunkDlsSample(Xe::IO::IStream* pStream, uint32_t size)
	{
		bool r;
		ASSERT(r = size >= sizeof(RIFFDLSSample));
		if (!r)
		{
			LOGE(LPREFIX"Unexpected length for DLS Sample chunk.");
			return false;
		}

		uint8_t* data = (uint8_t*)Xe::Memory::Alloc(size);
		ASSERT(pStream->Read(data, 0, size) == size);

		RIFFDLSSample& dlsSample = *(RIFFDLSSample*)data;
		ASSERT(r = (dlsSample.size + dlsSample.loopCount * sizeof(DLSLoop) <= size));
		if (!r)
		{
			LOGE(LPREFIX"Unexpected length for DLS Sample chunk.");
			return false;
		}

		DLSLoop* dlsLoops = (DLSLoop*)(data + dlsSample.size);
		for (uint32_t i = 0; i < dlsSample.loopCount; i++)
		{
			const DLSLoop& dlsLoop = dlsLoops[i];
			Loop loop;

			switch (dlsLoop.loopType)
			{
			case DLSLoop::LOOP_TYPE_FORWARD:
			case DLSLoop::LOOP_TYPE_RELEASE:
				loop.Start = loop.Start;
				loop.Length = loop.Length;
				break;
			}

			m_Loops.push_back(loop);
		}

		Xe::Memory::Free(data);
		return true;
	}

	bool WavAudioSource::ChunkMidiSample(Xe::IO::IStream* pStream, uint32_t size)
	{
		bool r;
		ASSERT(r = size >= sizeof(RIFFDLSSample));
		if (!r)
		{
			LOGE(LPREFIX"Unexpected length for MIDI Sample chunk.");
			return false;
		}

		uint8_t* data = (uint8_t*)Xe::Memory::Alloc(size);
		ASSERT(pStream->Read(data, 0, size) == size);

		RIFFMIDISample& midiSample = *(RIFFMIDISample*)data;
		ASSERT(r = (sizeof(RIFFMIDISample) + midiSample.loopCount * sizeof(MIDILoop) <= size));
		if (!r)
		{
			LOGE(LPREFIX"Unexpected length for MIDI Sample chunk.");
			return false;
		}

		MIDILoop* midiLoops = (MIDILoop*)(data + sizeof(RIFFMIDISample));
		for (uint32_t i = 0; i < midiSample.loopCount; i++)
		{
			const MIDILoop& midiLoop = midiLoops[i];
			Loop loop;

			switch (midiLoop.type)
			{
			case MIDILoop::LOOP_TYPE_FORWARD:
				loop.Start = loop.Start;
				loop.Length = loop.Length;
				break;
			}

			m_Loops.push_back(loop);
		}

		Xe::Memory::Free(data);
		return true;
	}

	bool WavAudioSource::ChunkXwmaDpds(Xe::IO::IStream* pStream, uint32_t size)
	{
		pStream->SetPosition(size, IO::Seek_Cur);
		return false;
	}

	bool WavAudioSource::ChunkXmaSeek(Xe::IO::IStream* pStream, uint32_t size)
	{
		pStream->SetPosition(size, IO::Seek_Cur);
		return false;
	}

	bool WavAudioSource::FormatPcm(PCMWAVEFORMAT& fmt)
	{
		switch (fmt.wBitsPerSample)
		{
		case 8: m_BitFormat = BitFormat_S8; break;
		case 16: m_BitFormat = BitFormat_S16L; break;
		case 24: m_BitFormat = BitFormat_S24L; break;
		case 32: m_BitFormat = BitFormat_S32L; break;
		default: m_BitFormat = BitFormat_Unspecified; break;
		}

		m_SampleSize = m_ChannelsCount * fmt.wBitsPerSample / 8;
		return true;
	}

	bool WavAudioSource::FormatAdpcm(WAVEFORMATEX& fmt)
	{
		return false;
	}

	bool WavAudioSource::FormatIeeeFloat(WAVEFORMAT& fmt)
	{
		m_BitFormat = BitFormat_F32;
		m_SampleSize = m_ChannelsCount * 32 / 8;
		return true;
	}

	bool WavAudioSource::FormatWmaAudio2(WAVEFORMATEX& fmt)
	{
		return false;
	}

	bool WavAudioSource::FormatWmaAudio3(WAVEFORMATEX& fmt)
	{
		return false;
	}

#if _XBOX_ONE
	bool WavAudioSource::FormatXma2(XMA2WAVEFORMATEX& fmt)
	{
		return false;
	}
#endif

	bool WavAudioSource::FormatExtendible(WAVEFORMATEXTENSIBLE& fmt)
	{
		bool r;

		switch (fmt.SubFormat.Data1)
		{
		// MS-ADPCM and XMA2 are not supported as WAVEFORMATEXTENSIBLE
		case WAVE_FORMAT_PCM:
			r = FormatPcm(*(PCMWAVEFORMAT*)&fmt);
			break;
		case WAVE_FORMAT_IEEE_FLOAT:
			r = FormatIeeeFloat(*(WAVEFORMAT*)&fmt);
			break;
		case WAVE_FORMAT_WMAUDIO2:
			r = FormatWmaAudio2(*(WAVEFORMATEX*)&fmt);
			break;
		case WAVE_FORMAT_WMAUDIO3:
			r = FormatWmaAudio3(*(WAVEFORMATEX*)&fmt);
			break;
		}

		return r;
	}

	void WavAudioSource::SetPositionCurrentSub(SampleOffset position)
	{
		m_pStream->SetPosition(m_DataPosition + GetPositionCurrent() * m_SampleSize);
	}

	svar WavAudioSource::ReadSub(float *data, svar offset, SampleOffset count)
	{
		uint32_t bps = GetBitsPerSample(m_BitFormat);
		m_pStream->SetPosition(m_DataPosition + m_posCur * m_ChannelsCount * bps / 8);

		if (m_BitFormat != BitFormat_F32)
		{
			svar len = (svar)count * m_format.NumberOfChannels * bps / 8;
			void *buf = Memory::Alloc(len);
			svar read = m_pStream->Read(buf, 0, len);
			svar samplesRead = read * 8 / bps;
			Convert(data + offset, buf, m_BitFormat, samplesRead);
			Memory::Free(buf);
			return samplesRead * 4;
		}

		return m_pStream->Read(data, offset, m_format.SampleLength * (s32)count);
	}

} }