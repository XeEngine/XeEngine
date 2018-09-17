#pragma once
#include <XeGame/IGameSoundEntity.h>
#include <vector>

namespace Xe { namespace Sound {
	interface IAudioBuffer;
	interface IAudioSource;
} }

namespace Xe { namespace Game {
	class CSoundManager;
	class CSoundEntity : public virtual ISoundEntity
	{
	protected:
		enum State
		{
			State_Unknown,
			State_Play,
			State_Pause,
			State_Stop,
			State_End
		};

		s64 m_CreationTime;
		CSoundManager& m_SoundManager;
		Xe::Sound::IAudioBuffer& m_Buffer;
		Xe::Sound::IAudioSource& m_Source;
		std::vector<SoundLoop> m_Loops;
		EaseFuncf m_EaseFunc;

		double m_Volume;
		double m_SrcVolume;
		double m_DstVolume;
		double m_Timer;
		double m_Speed;

		Xe::Math::Vector3f m_Position;
		State m_NextState;
		int m_LoopIndex;
		virtual float GetMasterVolume() const = 0;

		void InternalPlay();
		void InternalPause();
		void InternalStop();
	public:
		CSoundEntity(
			CSoundManager& soundManager,
			Xe::Sound::IAudioBuffer& buffer,
			Xe::Sound::IAudioSource& source);
		~CSoundEntity();

		s64 GetCreationTime() const;

		void Update(double deltaTime);

		float GetVolume() const;
		void SetVolume(float volume, float speed, EaseFuncf ease);

		const Xe::Math::Vector3f& GetPosition() const;
		void SetPosition(const Xe::Math::Vector3f& position);

		bool IsPlaying() const;
		void Play(float speed, EaseFuncf ease);
		void Pause(float speed, EaseFuncf ease);
		void Stop(float speed, EaseFuncf ease);

		void SetLoops(const SoundLoop* loops, int loopsCount);
		int GetLoopsCount() const;
		int GetLoopIndex() const;
		void SetNextLoopIndex();


		void UpdateVolume();
	};
} }
