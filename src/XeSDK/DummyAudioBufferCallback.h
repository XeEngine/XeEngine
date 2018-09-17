#pragma once
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	struct DummyAudioBufferCallback : public IAudioBufferCallback
	{
		void OnBufferRequred(IAudioBuffer *pBuffer, svar bytesRequired) { }
		void OnBufferProcessed() { }
	};
} }