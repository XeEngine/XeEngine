#pragma once
#include <XeSDK/ISoundAudioSource.h>

namespace Xe { namespace Sound {
	class GenericAudioSource: public IAudioSource
	{
	public:
		GenericAudioSource();
		virtual ~GenericAudioSource();

		const WaveDesc &GetDesc() const;
		SampleOffset GetSamplesCount() const;
		SampleOffset GetPosition() const;
		void SetPosition(SampleOffset position);
		SampleOffset GetLoopStart() const;
		void SetLoopStart(SampleOffset position);
		SampleOffset GetLoopEnd() const;
		void SetLoopEnd(SampleOffset position);

		int Read(void *data, int offset, int bytescount);

	protected:
		WaveDesc m_WaveDesc;
		SampleOffset m_SamplesCount;
		SampleOffset m_Position;
		SampleOffset m_LoopStart;
		SampleOffset m_LoopEnd;

		// Set the position
		virtual void SetPositionSub(SampleOffset position) = 0;

		// Read the specified amount of samples
		//! \return Number of bytes read.
		virtual int ReadSub(void *data, int offset, int length) = 0;
	};
} }