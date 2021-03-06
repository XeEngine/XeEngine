#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		enum PlayState {
			State_Playing,
			State_Paused,
			State_Stopped,
			State_End,
		};

		interface IPlayable : public IObject {
		public:
			IPlayable();

			PlayState GetPlayState() const;
			virtual void Play() = 0;
			virtual void Pause() = 0;
			virtual void Stop() = 0;
			virtual float GetVolume() = 0;
			virtual void SetVolume(float volume) = 0;
			virtual void GetChannelVolumes(int channels, float* volumes) = 0;
			virtual void SetChannelVolumes(int channels, const float* volumes) = 0;

		protected:
			PlayState m_playState;
		};
	}
}