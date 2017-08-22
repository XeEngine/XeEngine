#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeSound.h>
#include <XeSDK/XeDebugLog.h>

using namespace Xe::Debug;

namespace Xe {
	namespace Sound {
		class AudioWasapi : public IAudio {
		public:
			class XAudioBuffer : public IAudioBuffer {
			public:
				bool Query(IObject **obj, UID id) {
					switch (id) {
					case IAudioBuffer::ID:
					case IObject::ID:
						AddRef();
						*obj = nullptr;
						return true;
					}
					*obj = nullptr;
					return false;
				}
				XAudioBuffer(const WaveFormat &format, ICallback *pCallback) :
					IAudioBuffer(format, pCallback) {
				}
				void Submit(const float *data, svar length) {

				}
				void Play() {

				}
				void Stop() {

				}
			};

			bool Query(IObject **obj, UID id) {
				switch (id) {
				case IAudio::ID:
				case IObject::ID:
					AddRef();
					*obj = this;
					return true;
				}
				*obj = nullptr;
				return false;
			}
			AudioWasapi() {
			}
			bool Initialize() {
				return true;
			}
			bool SetSampleRate(svar sampleRate) {
				if (sampleRate % 100)
					return false;
				m_sampleRate = sampleRate;
				return true;
			}
			bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback) {
				*buffer = new XAudioBuffer(format, pCallback);
				return true;
			}
		};

		bool CreateWasapi(IAudio **pAudio) {
			AudioWasapi *pcAudio = new AudioWasapi;
			if (pcAudio->Initialize()) {
				*pAudio = pcAudio;
				return true;
			}
			*pAudio = nullptr;
			return false;
		}
	}
}