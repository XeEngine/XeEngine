#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundEntity.h"
#include "CGameSoundManager.h"

namespace Xe { namespace Game {
	CSoundEntity::CSoundEntity(
		CSoundManager& soundManager,
		Xe::Sound::IAudioBuffer& buffer,
		Xe::Sound::IAudioSource& source) :
		m_CreationTime(Xe::Timer::Current().AsInteger()),
		m_SoundManager(soundManager),
		m_Buffer(buffer),
		m_Source(source),
		m_EaseFunc(Xe::Game::Ease::One),
		m_Volume(1.0f),
		m_SrcVolume(1.0f),
		m_DstVolume(1.0f),
		m_Timer(1.0f),
		m_Speed(0.0f),
		m_NextState(State_Unknown),
		m_LoopIndex(0)
	{
		buffer.AddRef();
		source.AddRef();
	}

	CSoundEntity::~CSoundEntity()
	{
		m_Buffer.Release();
		m_Source.Release();
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
				double t = m_EaseFunc(m_Timer);
				m_Volume = Xe::Math::Lerp(t, m_SrcVolume, m_DstVolume);
				UpdateVolume();
			}
		}
	}

	float CSoundEntity::GetVolume() const
	{
		return (float)m_Volume;
	}

	void CSoundEntity::SetVolume(float volume, float speed, EaseFuncf ease)
	{
		m_SrcVolume = m_Volume;
		m_DstVolume = volume;
		m_Speed = speed;
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
		return m_Buffer.IsPlaying();
	}

	void CSoundEntity::Play(float speed, EaseFuncf ease)
	{
		InternalPlay();
		if (speed > 0.0f)
		{
			SetVolume(1.0f, speed, ease);
		}

		m_NextState = State_Play;
	}

	void CSoundEntity::Pause(float speed, EaseFuncf ease)
	{
		if (speed > 0.0f)
		{
			m_NextState = State_Pause;
			SetVolume(0.0f, speed, ease);
		}
		else
		{
			InternalPause();
		}
	}

	void CSoundEntity::Stop(float speed, EaseFuncf ease)
	{
		if (speed > 0.0f)
		{
			m_NextState = State_Stop;
			SetVolume(0.0f, speed, ease);
		}
		else
		{
			InternalStop();
		}
	}

	u64 CSoundEntity::GetSamplesPosition() const
	{
		return m_Source.GetPosition();
	}
	void CSoundEntity::SetSamplesPosition(u64 position)
	{
		m_Source.SetPosition(position);
	}

	void CSoundEntity::SetLoops(const SoundLoop* loops, int loopsCount)
	{
		m_LoopIndex = 0;

		m_Loops = std::vector<SoundLoop>(loopsCount);
		for (int i = 0; i < loopsCount; i++)
		{
			m_Loops[i] = loops[i];
		}

		if (loopsCount > 0)
		{
			m_Source.SetLoopStart(m_Loops[0].Start);
			m_Source.SetLoopEnd(m_Loops[0].End);
		}
		else
		{
			auto samplesCount = m_Source.GetSamplesCount();
			m_Source.SetLoopStart(samplesCount);
			m_Source.SetLoopEnd(samplesCount);
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
		if (m_LoopIndex + 1 < GetLoopsCount())
		{
			const SoundLoop& loop = m_Loops[++m_LoopIndex];
			m_Source.SetLoopStart(loop.Start);
			m_Source.SetLoopEnd(loop.End);
		}
	}


	void CSoundEntity::UpdateVolume()
	{
		float volume = (float)(m_Volume * GetMasterVolume());
		m_Buffer.SetVolume(volume);

		//const auto& listenerPos = m_SoundManager.GetListenerPosition();
		//float listenerRad = m_SoundManager.GetListenerRange();
		//float xd = m_Position.x - listenerPos.x;

		//float volumes[2]{ volume, volume };
		//m_Buffer.SetChannelVolumes(lengthof(volumes), volumes);
	}

	void CSoundEntity::InternalPlay()
	{
		m_Buffer.Play();
	}

	void CSoundEntity::InternalPause()
	{
		m_Buffer.Stop();
	}

	void CSoundEntity::InternalStop()
	{
		m_Buffer.Stop();
		m_Source.SetPosition(0);
	}
} }