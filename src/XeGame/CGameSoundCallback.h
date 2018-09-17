#pragma once
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	interface IAudioSource;
} }
namespace Xe { namespace Game {
	struct CGameSoundEntity;
	struct CSoundCallback : public Xe::Sound::IAudioBufferCallback
	{
		Xe::Sound::IAudioSource& m_AudioSource;
		void* m_Buffer;
		int m_BufferLength;

		CSoundCallback(Xe::Sound::IAudioSource& audioSource);
		~CSoundCallback();

		void OnBufferRequred(Xe::Sound::IAudioBuffer *pBuffer, svar bytesRequired);
		void OnBufferProcessed();
	};
} }
