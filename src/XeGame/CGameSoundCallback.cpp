#include "pch.h"
#include <XeSDK/XeMemory.h>
#include "CGameSoundCallback.h"
#include "CGameSoundEntity.h"

namespace Xe { namespace Game {
	CSoundCallback::CSoundCallback(
		Xe::Sound::IAudioSource& audioSource) :
		m_AudioSource(audioSource),
		m_Buffer(nullptr),
		m_BufferLength(0)
	{
		audioSource.AddRef();
	}

	CSoundCallback::~CSoundCallback()
	{
		if (m_Buffer) Xe::Memory::Free(m_Buffer);
		m_AudioSource.Release();
	}

	void CSoundCallback::OnBufferRequred(Xe::Sound::IAudioBuffer *pBuffer, svar bytesRequired)
	{
		if (m_BufferLength < bytesRequired)
		{
			m_BufferLength = bytesRequired;
			m_Buffer = Xe::Memory::Resize(m_Buffer, bytesRequired);
			LOGFA(!!m_Buffer);
		}

		int bytesRead = m_AudioSource.Read(m_Buffer, 0, bytesRequired);
		if (bytesRead > 0)
		{
			pBuffer->Submit(m_Buffer, bytesRead);
		}
		else
		{
			pBuffer->Stop();
		}
	}

	void CSoundCallback::OnBufferProcessed()
	{

	}
} }