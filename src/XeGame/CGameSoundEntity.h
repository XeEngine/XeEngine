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
		Xe::Sound::IAudioBuffer* m_Buffer;
		Xe::Sound::IAudioSource* m_Source;
		std::vector<SoundLoop> m_Loops;
		EaseFuncf m_EaseFunc;

		Xe::Sound::IAudioBufferCallback* m_Callback;
		void* m_DataBuffer;
		int m_BufferLength;

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
		CSoundEntity(CSoundManager& soundManager);
		~CSoundEntity();

		bool SetAudioSource(Xe::Sound::IAudioSource& source);
		s64 GetCreationTime() const;

		void Update(double deltaTime);

		float GetVolume() const;
		void SetVolume(float volume, float seconds, EaseFuncf ease);

		const Xe::Math::Vector3f& GetPosition() const;
		void SetPosition(const Xe::Math::Vector3f& position);

		bool IsPlaying() const;
		void Play(float seconds, EaseFuncf ease);
		void Pause(float seconds, EaseFuncf ease);
		void Stop(float seconds, EaseFuncf ease);

		u64 GetSamplesCount() const;
		u64 GetSamplesPosition() const;
		void SetSamplesPosition(u64 position);

		void SetLoops(const SoundLoop* loops, int loopsCount);
		int GetLoopsCount() const;
		int GetLoopIndex() const;
		void SetNextLoopIndex();

		void UpdateVolume();

		// IAudioBufferCallback
		void OnBufferRequred(Xe::Sound::IAudioBuffer *pBuffer, svar bytesRequired);
		void OnBufferProcessed();
	};
} }
