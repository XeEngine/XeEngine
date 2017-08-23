#include "pch.h"
#include <XeSDK/ISoundAudioSource.h>
#include <XeSDK/ISoundAudioEffect.h>

namespace Xe {
	namespace Sound {
		IAudioSource::IAudioSource() :
			m_posEnd(0),
			m_posCur(0),
			m_posLoop((u64)-1)
		{ }

		const WaveFormat &IAudioSource::GetFormat() const {
			return m_format;
		}

		SampleOffset IAudioSource::GetPositionEnd() const {
			return m_posEnd;
		}

		SampleOffset IAudioSource::GetPositionCurrent() const {
			return m_posCur;
		}

		void IAudioSource::SetPositionCurrent(SampleOffset position) {
			m_posCur = position;
			SetPositionCurrentSub(position);
		}

		SampleOffset IAudioSource::GetPositionLoop() const {
			return m_posLoop;
		}

		void IAudioSource::SetPositionLoop(SampleOffset position) {
			m_posLoop = position;
		}

		void IAudioSource::SetEffect(IEffect *effect) {
			if (m_pEffect != nullptr)
				m_pEffect->Release();
			m_pEffect = effect;
			m_pEffect->AddRef();
		}

		svar IAudioSource::Read(float *data, svar offset, svar bytescount) {
			SampleOffset samplesRequired = bytescount / m_format.SampleLength;
			SampleOffset len = 0;
			while (len < samplesRequired) {
				if (GetPositionCurrent() >= GetPositionEnd()) {
					if (GetPositionLoop() >= GetPositionEnd())
						break;
					SetPositionCurrent(GetPositionLoop());
				}
				s64 diff = (s64)(GetPositionEnd() - (GetPositionCurrent() + (samplesRequired - len)));
				SampleOffset toRead = (diff < 0) ? GetPositionEnd() - GetPositionCurrent() : (samplesRequired - len);
				svar read = ReadSub(data, (svar)(len * m_format.SampleLength), toRead);
				if (read == 0)
					break;
				SampleOffset samplesRead = read / m_format.SampleLength;
				SetPositionCurrent(GetPositionCurrent() + samplesRead);
				len += samplesRead;
			}
			if (m_pEffect != nullptr && len > 0) {
				//m_pEffect
			}
			return (svar)(len * m_format.SampleLength);
		}
	}
}