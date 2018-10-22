#include "pch.h"
#include "OggAudioSource.h"
#include <XeSDK/XeSound.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeLogger.h>

namespace Xe { namespace Sound {
	OggAudioSource::OggAudioSource() :
		m_pStream(nullptr),
		m_CurrentSection(0),
		m_Eof(false)
	{
	}

	OggAudioSource::~OggAudioSource()
	{
		ov_clear(&m_Vorbis);
		if (m_pStream) m_pStream->Release();
	}


	bool OggAudioSource::Initialize(Xe::IO::IStream* pStream)
	{
		m_pStream = pStream;
		m_pStream->AddRef();

		ov_callbacks callbacks;
		callbacks.read_func = ReadFunc;
		callbacks.seek_func = m_pStream->CanSeek() ? SeekFunc : nullptr;
		callbacks.close_func = nullptr;
		callbacks.tell_func = m_pStream->CanSeek() ? TellFunc : nullptr;

		int r = ov_open_callbacks(pStream, &m_Vorbis, NULL, 0, callbacks);
		if (r < 0)
		{
			switch (r)
			{
			case 0:
				break;
			case OV_EREAD:
				LOGE("Read error");
				break;
			case OV_ENOTVORBIS:
				LOGE("Stream does not contain any Vorbis data");
				break;
			case OV_EVERSION:
				LOGE("Vorbis version mismatch");
				break;
			case OV_EBADHEADER:
				LOGE("Invalid Vorbis header");
				break;
			case OV_EFAULT:
				LOGE("Internal error");
				break;
			default:
				LOGE("Error %i", r);
				break;
			}

			return false;
		}

		const vorbis_info& info = *ov_info(&m_Vorbis, -1);
		m_format.WaveFormat = WaveType_Pcm;
		m_format.BitDepth = BitFormat_S16L;
		m_format.SampleRate = info.rate;
		m_format.SampleLength = info.channels * GetBitsPerSample(m_format.BitDepth) / 8;
		m_format.NumberOfChannels = info.channels;

		ogg_int64_t samplesCount = ov_pcm_total(&m_Vorbis, -1);
		if (samplesCount == OV_EINVAL)
		{
			LOGW("Seek not supported");
			samplesCount = 0;
		}

		m_posEnd = samplesCount;

		return true;
	}


	void OggAudioSource::SetPositionCurrentSub(SampleOffset position)
	{
		ov_pcm_seek(&m_Vorbis, position);
	}

	svar OggAudioSource::ReadSub(void *data, svar offset, SampleOffset count)
	{
		long read = 0;
		if (!m_Eof)
		{
			read = ov_read(&m_Vorbis, (char*)data + offset, count * m_format.SampleLength, 0, 2, 1, &m_CurrentSection);
			if (read > 0)
			{
				// Ok!
			}
			else if (read == 0)
			{
				m_Eof = true;
			}
			else //if (ret < 0)
			{
				// Corrupted stream, recoverable error.
				read = 0;
			}
		}

		return read;
	}

	size_t OggAudioSource::ReadFunc(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		return ((Xe::IO::IStream*)datasource)->Read(ptr, 0, size * nmemb);
	}

	int OggAudioSource::SeekFunc(void *datasource, ogg_int64_t offset, int whence)
	{
		return ((Xe::IO::IStream*)datasource)->SetPosition(offset, (Xe::IO::Seek)whence);
	}

	long OggAudioSource::TellFunc(void *datasource)
	{
		return ((Xe::IO::IStream*)datasource)->GetPosition();
	}

} }