#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundEntity.h"
#include "CGameSoundManager.h"

namespace Xe { namespace Game {
	struct CAudioBufferCallback : Xe::Sound::IAudioBufferCallback
	{
		CSoundEntity& m_SoundEntity;

		CAudioBufferCallback(CSoundEntity& soundEntity) :
			m_SoundEntity(soundEntity)
		{ }

		void OnBufferRequred(Xe::Sound::IAudioBuffer *pBuffer, svar bytesRequired)
		{
			m_SoundEntity.OnBufferRequred(pBuffer, bytesRequired);
		}

		void OnBufferProcessed()
		{
			m_SoundEntity.OnBufferProcessed();
		}
	};

	CSoundEntity::CSoundEntity(
		CSoundManager& soundManager) :
		m_CreationTime(0),
		m_SoundManager(soundManager),
		m_Buffer(nullptr),
		m_Source(nullptr),
		m_EaseFunc(Xe::Game::Ease::One),
		m_DataBuffer(nullptr),
		m_BufferLength(0),
		m_Volume(1.0f),
		m_SrcVolume(1.0f),
		m_DstVolume(1.0f),
		m_Timer(1.0f),
		m_Speed(0.0f),
		m_NextState(State_Unknown),
		m_LoopIndex(0)
	{
		m_Callback = new CAudioBufferCallback(*this);
	}

	CSoundEntity::~CSoundEntity()
	{
		if (m_Buffer) m_Buffer->Release();
		if (m_Source) m_Source->Release();
		if (m_DataBuffer) Xe::Memory::Free(m_DataBuffer);
		delete m_Callback;
	}

	bool CSoundEntity::SetAudioSource(Xe::Sound::IAudioSource& source)
	{
		if (m_Source != &source)
		{
			if (m_Source)
				m_Source->Release();

			m_Source = &source;
			m_Source->AddRef();

			const auto& desc = source.GetDesc();
			if (!!m_Buffer && m_Buffer->GetFormat() != desc)
			{
				delete m_Buffer;
				m_Buffer = nullptr;
			}

			if (!m_Buffer)
			{
				if (!m_SoundManager.m_Audio.CreateBuffer(&m_Buffer, desc))
				{
					LOGE("Unable to create the IAudioBuffer from the specified IAudioSource's desc.");
					return false;
				}
				else
				{
					m_Buffer->SetCallback(*m_Callback);
				}
			}
		}

		m_CreationTime = Timer::Current().AsInteger();
		m_EaseFunc = Xe::Game::Ease::One;
		m_Volume = m_SrcVolume = m_DstVolume = 1.0f;
		m_Timer = 1.0f;
		m_Speed = 0.0f;
		m_NextState = State_Unknown;
		m_LoopIndex = 0;
		UpdateVolume();

		return true;
	}

	s64 CSoundEntity::GetCreationTime() const
	{
		return m_CreationTime;
	}

	void CSoundEntity::Update(double deltaTime)
	{
		if (m_Timer < 1.0)
		{
			m_Timer += m_Speed * deltaTime;
			if (m_Timer >= 1.0)
			{
				m_Timer = 1.0;
				m_Volume = m_DstVolume;

				switch (m_NextState)
				{
				case State_Pause:
					InternalPause();
					break;
				case State_Stop:
					InternalStop();
					break;
				}
			}
			else
			{
				double t = m_EaseFunc((float)m_Timer);
				m_Volume = Xe::Math::Lerp(t, m_SrcVolume, m_DstVolume);
				UpdateVolume();
			}
		}
	}

	float CSoundEntity::GetVolume() const
	{
		return (float)m_Volume;
	}

	void CSoundEntity::SetVolume(float volume, float seconds, EaseFuncf ease)
	{
		m_SrcVolume = m_Volume;
		m_DstVolume = volume;
		m_Speed = 1.0 / seconds;
		m_Timer = 0.0;
		m_EaseFunc = ease;
	}

	const Xe::Math::Vector3f& CSoundEntity::GetPosition() const
	{
		return m_Position;
	}

	void CSoundEntity::SetPosition(const Xe::Math::Vector3f& position)
	{
		m_Position = position;
	}

	bool CSoundEntity::IsPlaying() const
	{
		LOGFA(m_Buffer);
		return m_Buffer->IsPlaying();
	}

	void CSoundEntity::Play(float seconds, EaseFuncf ease)
	{
		InternalPlay();
		if (seconds > 0.0f)
		{
			SetVolume(1.0f, seconds, ease);
		}

		m_NextState = State_Play;
	}

	void CSoundEntity::Pause(float seconds, EaseFuncf ease)
	{
		if (seconds > 0.0f)
		{
			m_NextState = State_Pause;
			SetVolume(0.0f, seconds, ease);
		}
		else
		{
			InternalPause();
		}
	}

	void CSoundEntity::Stop(float seconds, EaseFuncf ease)
	{
		if (IsPlaying() && seconds > 0.0f)
		{
			m_NextState = State_Stop;
			SetVolume(0.0f, seconds, ease);
		}
		else
		{
			InternalStop();
		}
	}

	u64 CSoundEntity::GetSamplesCount() const
	{
		LOGFA(m_Source);
		return m_Source->GetSamplesCount();
	}

	u64 CSoundEntity::GetSamplesPosition() const
	{
		LOGFA(m_Source);
		return m_Source->GetPosition();
	}
	void CSoundEntity::SetSamplesPosition(u64 position)
	{
		LOGFA(m_Source);
		m_Source->SetPosition(position);
	}

	void CSoundEntity::SetLoops(const SoundLoop* loops, int loopsCount)
	{
		LOGFA(m_Source);
		m_LoopIndex = 0;

		m_Loops = std::vector<SoundLoop>(loopsCount);
		for (int i = 0; i < loopsCount; i++)
		{
			m_Loops[i] = loops[i];
		}

		if (loopsCount > 0)
		{
			m_Source->SetLoopStart(m_Loops[0].Start);
			m_Source->SetLoopEnd(m_Loops[0].End);
		}
		else
		{
			auto samplesCount = m_Source->GetSamplesCount();
			m_Source->SetLoopStart(samplesCount);
			m_Source->SetLoopEnd(samplesCount);
		}
	}

	int CSoundEntity::GetLoopsCount() const
	{
		return m_Loops.size();
	}

	int CSoundEntity::GetLoopIndex() const
	{
		return m_LoopIndex;
	}

	void CSoundEntity::SetNextLoopIndex()
	{
		LOGFA(m_Source);
		if (m_LoopIndex + 1 < GetLoopsCount())
		{
			const SoundLoop& loop = m_Loops[++m_LoopIndex];
			m_Source->SetLoopStart(loop.Start);
			m_Source->SetLoopEnd(loop.End);
		}
	}

	void CSoundEntity::OnBufferRequred(Xe::Sound::IAudioBuffer *pBuffer, svar bytesRequired)
	{
		if (m_BufferLength < bytesRequired)
		{
			m_BufferLength = bytesRequired;
			m_DataBuffer = Xe::Memory::Resize(m_DataBuffer, bytesRequired);
			LOGFA(!!m_DataBuffer);
		}

		int bytesRead = m_Source->Read(m_DataBuffer, 0, bytesRequired);
		if (bytesRead > 0)
		{
			pBuffer->Submit(m_DataBuffer, bytesRead);
		}
		else
		{
			pBuffer->Stop();
		}
	}

	void CSoundEntity::OnBufferProcessed()
	{ }

	void CSoundEntity::UpdateVolume()
	{
		LOGFA(m_Buffer);

		float volume = (float)(m_Volume * GetMasterVolume());
		m_Buffer->SetVolume(volume);

		//const auto& listenerPos = m_SoundManager.GetListenerPosition();
		//float listenerRad = m_SoundManager.GetListenerRange();
		//float xd = m_Position.x - listenerPos.x;

		//float volumes[2]{ volume, volume };
		//m_Buffer.SetChannelVolumes(lengthof(volumes), volumes);
	}

	void CSoundEntity::InternalPlay()
	{
		LOGFA(m_Buffer);
		m_Buffer->Play();
	}

	void CSoundEntity::InternalPause()
	{
		LOGFA(m_Buffer);
		m_Buffer->Stop();
	}

	void CSoundEntity::InternalStop()
	{
		LOGFA(m_Buffer);
		m_Buffer->Stop();
		m_Source->SetPosition(0);
	}
} }