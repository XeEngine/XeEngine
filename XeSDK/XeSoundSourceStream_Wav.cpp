#include "pch.h"
#include <XeSDK/XeSoundAudioSource.h>
#include <XeSDK/ISound.h>
#include <XeSDK/ISoundAudioSource.h>
#include <XeSDK/ISoundPlayable.h>
#include <XeSDK/XeSound.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeDebugLog.h>

#define WAVE_FORMAT_PCM 1 
#define WAVE_FORMAT_IEEE_FLOAT 3

using namespace Xe::Debug;

namespace Xe {
	namespace Sound {
		// https://www.aelius.com/njh/wavemetatools/doc/riffmci.pdf
		class SourceStreamWav : public IAudioSource {

			typedef u32 FOURCC;
			typedef FOURCC CKID;
			typedef u32 CKSIZE;

			struct Header
			{
				u32 chunkId;
				s32 chunkSize;
				u32 fileFormat;
			};
			struct Chunk
			{
				FOURCC id;
				CKSIZE length;
			};
			// https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx
			struct WAVEFORMATEX
			{
				u16 formatTag;
				u16 nChannels;
				u32 samplesPerSec;
				u32 avgBytesPerSec;
				u16 blockAlign;
				u16 bitsPerSample;
				u16 cbSize;
			};

			bool Query(IObject **obj, UID id) {
				switch (id) {
				case IAudioSource::ID:
				case IObject::ID:
					AddRef();
					*obj = this;
					return true;
				}
				*obj = nullptr;
				return false;
			}

			void SetPositionCurrentSub(SampleOffset position) {
				m_pStream->SetPosition(m_pcmPosition + GetPositionCurrent() * m_internalSampleBlock);
			}

			svar ReadSub(float *data, svar offset, SampleOffset count) {
				if (m_bitFormat != BitFormat_F32) {
					svar len = (svar)count * m_format.NumberOfChannels * m_bps / 8;
					void *buf = Memory::Alloc(len);
					svar read = m_pStream->Read(buf, 0, len);
					svar samplesRead = read * 8 / m_bps;
					Convert(data + offset, buf, m_bitFormat, samplesRead);
					Memory::Free(buf);
					return samplesRead * 4;
				}
				else
					return m_pStream->Read(data, offset, m_format.SampleLength * (s32)count);
			}

		public:
			SourceStreamWav() :
				m_pStream(nullptr)
			{
			}
			~SourceStreamWav() {
				if (m_pStream)
					m_pStream->Release();
			}
			bool Open(IO::IStream *pStream) {
				static const FOURCC ID_RIFF = 0x46464952;
				static const FOURCC ID_WAVE = 0x45564157;
				static const FOURCC CNKID_fmt_ = 0x20746D66;
				static const FOURCC CNKID_data = 0x61746164;

				if (!pStream->CanRead()) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Specified stream %p does not support reading."), pStream);
					return false;
				}

				Header header;
				if (pStream->Read(&header, 0, 12) != 12) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Unable to read WAV header from stream %p."), pStream);
					return false;
				}
				if (header.chunkId != ID_RIFF) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Invalid header for WAV file from stream %p."), pStream);
					return false;
				}
				if (header.chunkSize > pStream->GetLength() - pStream->GetPosition() + 4) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Invalid size for WAV file from stream %p."), pStream);
					return false;
				}
				if (header.fileFormat != ID_WAVE) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Unsupported CHID %c%c%c%c for WAV file from stream %p."),
						((u8*)header.chunkId)[0], ((u8*)header.chunkId)[1], ((u8*)header.chunkId)[2], ((u8*)header.chunkId)[3], pStream);
					return false;
				}

				header.chunkSize -= 4;

				WAVEFORMATEX waveFormatEx;
				while (header.chunkSize > 0) {
					Chunk chunk;
					pStream->Read(&chunk, 0, 8);
					switch (chunk.id) {
					case CNKID_fmt_:
						pStream->Read(&waveFormatEx, 0, chunk.length);
						switch (waveFormatEx.formatTag) {
						case WAVE_FORMAT_PCM:
							switch (waveFormatEx.bitsPerSample) {
							case 8: m_bitFormat = BitFormat_S8; break;
							case 16: m_bitFormat = BitFormat_S16L; break;
							case 24: m_bitFormat = BitFormat_S24L; break;
							case 32: m_bitFormat = BitFormat_S32L; break;
							default: m_bitFormat = BitFormat_Unspecified; break;
							}
							m_bps = GetBitsPerSample(m_bitFormat);
							m_internalSampleBlock = waveFormatEx.nChannels * m_bps / 8;
							break;
						case WAVE_FORMAT_IEEE_FLOAT:
							m_bitFormat = BitFormat_F32;
							m_bps = 32;
							m_internalSampleBlock = waveFormatEx.nChannels * sizeof(float);
							break;
						}
						if (waveFormatEx.formatTag != WAVE_FORMAT_PCM) {
							LOG(Log::Priority_Error, Log::Type_Sound, _T("Stream %p it's not a WAV PCM."), pStream);
							return false;
						}
						m_internalSampleBlock = waveFormatEx.nChannels * m_bps / 8;
						m_format.NumberOfChannels = waveFormatEx.nChannels;
						m_format.SampleRate = waveFormatEx.samplesPerSec;
						m_format.SampleLength = waveFormatEx.nChannels * sizeof(float);
						break;
					case CNKID_data:
						pStream->Read(&waveFormatEx, 0, chunk.length);
						m_pcmPosition = pStream->GetPosition();
						m_posEnd = chunk.length / m_internalSampleBlock;
						break;
					}
					header.chunkSize -= chunk.length + 8;
				}

				pStream->AddRef();
				m_pStream = pStream;
				SetPositionCurrent(0);
				return true;
			}

		private:
			IO::IStream *m_pStream;
			s64 m_pcmPosition;
			BitFormat m_bitFormat;
			svar m_bps;
			svar m_internalSampleBlock;
		};

		bool OpenSourceStreamWav(IAudioSource **pAudioSource, IO::IStream *pStream) {
			SourceStreamWav *wav = new SourceStreamWav;
			if (wav->Open(pStream)) {
				*pAudioSource = wav;
				return true;
			}
			*pAudioSource = nullptr;
			return false;
		}
	}
}