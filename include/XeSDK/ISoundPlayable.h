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

		class IPlayable : public IObject {
		public:
			static const UID ID = 0xae945bd9671a4758ULL;

			IPlayable();

			PlayState GetPlayState() const;
			virtual void Play() = 0;
			virtual void Pause() = 0;
			virtual void Stop() = 0;

		protected:
			PlayState m_playState;
		};
	}
}