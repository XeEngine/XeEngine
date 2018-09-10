#include "pch.h"
#include <XeSDK/XeSoundGenericAudioSource.h>

namespace Xe {
	namespace Sound {
		GenericAudioSource::GenericAudioSource() :
			m_SamplesCount(0),
			m_Position(0),
			m_LoopStart(0),
			m_LoopEnd(0x7FFFFFFF)
		{ }

		GenericAudioSource::~GenericAudioSource()
		{

		}

		const WaveDesc &GenericAudioSource::GetDesc() const
		{
			return m_WaveDesc;
		}

		SampleOffset GenericAudioSource::GetSamplesCount() const
		{
			return m_SamplesCount;
		}

		SampleOffset GenericAudioSource::GetPosition() const
		{
			return m_Position;
		}

		void GenericAudioSource::SetPosition(SampleOffset position)
		{
			m_Position = position;
			SetPositionSub(position);
		}

		SampleOffset GenericAudioSource::GetLoopStart() const
		{
			return m_LoopStart;
		}

		void GenericAudioSource::SetLoopStart(SampleOffset position)
		{
			m_LoopStart = position;
		}

		SampleOffset GenericAudioSource::GetLoopEnd() const
		{
			return m_LoopStart;
		}

		void GenericAudioSource::SetLoopEnd(SampleOffset position)
		{
			m_LoopEnd = position;
		}

		int GenericAudioSource::Read(void *data, int offset, int bytesCount)
		{
			SampleOffset samplesRequired = bytesCount / m_WaveDesc.SampleLength;
			s64 totalRead = 0;

			while (samplesRequired > 0 && totalRead < bytesCount)
			{
				if (m_Position >= m_LoopEnd && m_LoopEnd <= m_SamplesCount)
				{
					SetPosition(m_LoopStart);
				}

				if (m_Position >= m_SamplesCount)
				{
					break; // end of file
				}

				SampleOffset samplesToRead = samplesRequired;
				SampleOffset samplesToLoopEnd = m_LoopEnd - m_Position;
				if (samplesRequired > samplesToLoopEnd)
				{
					samplesToRead = samplesToLoopEnd;
				}

				if (samplesToRead == 0)
					break;

				int read = ReadSub((u8*)data + totalRead, 0, (int)(samplesToRead * m_WaveDesc.SampleLength));
				if (read == 0)
					break;

				SampleOffset offsetRead = read / m_WaveDesc.SampleLength;
				totalRead += read;
				m_Position += offsetRead;
				samplesRequired -= offsetRead;
			}
			
			return totalRead;
		}
	}
}