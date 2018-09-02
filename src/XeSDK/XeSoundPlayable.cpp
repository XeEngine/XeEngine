#include "pch.h"
#include <XeSDK/XeSoundPlayable.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
	namespace Sound {
		IPlayable::IPlayable() :
			m_playState(State_Stopped) {}
		PlayState IPlayable::GetPlayState() const {
			return m_playState;
		}

		class CPlayable;
		struct CCallback : public IAudioBuffer::ICallback {
			static const svar BUFFERSCOUNT = 2;
			CPlayable *Playable;
			svar BuffersUsed;
			svar BufferIndex;
			svar BufferLen[BUFFERSCOUNT];
			float *BufferData[BUFFERSCOUNT];
			bool IsRunning;

			CCallback() :
				Playable(nullptr),
				BuffersUsed(0),
				BufferIndex(0),
				IsRunning(false)
			{
				Memory::Fill(BufferLen, 0, sizeof(BufferLen));
				Memory::Fill(BufferData, 0, sizeof(BufferData));
			}
			~CCallback() {
				for (svar i = 0; i < BUFFERSCOUNT; i++) {
					if (BufferData[i] != nullptr)
						Memory::Free(BufferData[i]);
				}
			}
			void SetPlayable(CPlayable *playable);
			void OnBufferRequred(IAudioBuffer *pBuffer, svar bytesRequired);
			void OnBufferProcessed();
		};

		class CPlayable : public IPlayable {
		public:
			IAudioBuffer *m_soundBuffer;
			IAudioSource *m_audioSource;
			CCallback *m_callback;

			void Play() {
				m_playState = State_Playing;
				m_soundBuffer->Play();
			}
			void Pause() {
				m_playState = State_Paused;
				m_soundBuffer->Stop();
			}
			void Stop() {
				m_playState = State_Stopped;
				m_soundBuffer->Stop();
				m_audioSource->SetPositionCurrent(0);
			}
			void End() {
				m_playState = State_End;
			}

			float GetVolume() {
				return m_soundBuffer->GetVolume();
			}

			void SetVolume(float volume) {
				m_soundBuffer->SetVolume(volume);
			}

			void GetChannelVolumes(int channels, float* volumes) {
				m_soundBuffer->GetChannelVolumes(channels, volumes);
			}

			void SetChannelVolumes(int channels, const float* volumes) {
				m_soundBuffer->SetChannelVolumes(channels, volumes);
			}

			CPlayable(IAudioBuffer *soundBuffer, IAudioSource *audioSource, CCallback* callback) :
				m_soundBuffer(soundBuffer),
				m_audioSource(audioSource),
				m_callback(callback) {

				m_soundBuffer->AddRef();
				m_audioSource->AddRef();
			}

			~CPlayable() {
				Stop();
				while (m_callback->IsRunning);
				m_audioSource->Release();
				m_soundBuffer->Release();
				delete m_callback;
			}
		};

		void CCallback::SetPlayable(CPlayable *playable) {
			Playable = playable;
		}

		void CCallback::OnBufferRequred(IAudioBuffer *pBuffer, svar bytesRequired) {
			IsRunning = true;
			if (BuffersUsed < BUFFERSCOUNT) {
				if (BufferLen[BufferIndex] < bytesRequired) {
					BufferLen[BufferIndex] = bytesRequired * Playable->m_audioSource->GetFormat().SampleLength;
					if (BufferData[BufferIndex] != nullptr)
						Memory::Free(BufferData[BufferIndex]);
					BufferData[BufferIndex] = (float*)Memory::Alloc(BufferLen[BufferIndex]);
				}
				svar len = Playable->m_audioSource->Read(BufferData[BufferIndex], 0, bytesRequired);
				if (len > 0) {
					pBuffer->Submit(BufferData[BufferIndex], len);
					BuffersUsed++;
				}
				else
					Playable->End();
			}
			IsRunning = false;
		}

		void CCallback::OnBufferProcessed() {
			BuffersUsed--;
		}

		bool CreatePlayable(IPlayable **pPlayable, IAudio *audio, IAudioSource *src) {
			if (audio == nullptr)
			{
				return false;
			}

			IAudioBuffer *dst;
			CCallback *callback = new CCallback;
			if (audio->CreateBuffer(&dst, src->GetFormat(), callback)) {
				*pPlayable = new CPlayable(dst, src, callback);
				dst->Release();
				callback->SetPlayable((CPlayable*)*pPlayable);
				return true;
			}
			else {
				audio->Release();
				delete callback;
				return false;
			}
			return true;
		}
	}
}