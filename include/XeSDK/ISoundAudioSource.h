#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/ISound.h>

namespace Xe {
	namespace Sound {
		typedef u64 SampleOffset;

		interface IEffect;
		class IAudioSource: public IObject {
		public:
			IAudioSource();

			const WaveDesc &GetFormat() const;
			SampleOffset GetPositionEnd() const;
			SampleOffset GetPositionCurrent() const;
			void SetPositionCurrent(SampleOffset position);
			SampleOffset GetPositionLoop() const;
			void SetPositionLoop(SampleOffset position);

			void SetEffect(IEffect *effect);
			svar Read(float *data, svar offset, svar bytescount);

		protected:
			u64 m_posEnd;
			u64 m_posCur;
			u64 m_posLoop;
			WaveDesc m_format;
			IEffect *m_pEffect;

			virtual void SetPositionCurrentSub(SampleOffset position) = 0;
			virtual svar ReadSub(float *data, svar offset, SampleOffset count) = 0;
		};
	}
}