#pragma once
#include "XAudioDefinitions.h"
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {

	class XAudioEngine;

	class XAudioBuffer : public IAudioBuffer {
	public:
		struct CXAudio2VoiceCallback : public IXAudio2VoiceCallback {
			static const svar MaxBuffersCount = 2;
			IAudioBuffer *pBuffer;
			ICallback *m_pCallback;

			CXAudio2VoiceCallback(XAudioBuffer *cBuffer, ICallback *pCallback) :
				pBuffer(cBuffer),
				m_pCallback(pCallback) {
			}

			// Called just before this voice's processing pass begins.
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceProcessingPassStart(%i)"), BytesRequired);
				m_pCallback->OnBufferRequred(pBuffer, BytesRequired);
			}

			// Called just after this voice's processing pass ends.
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceProcessingPassEnd()"));
				m_pCallback->OnBufferProcessed();
			}

			// Called when this voice has just finished playing a buffer stream
			// (as marked with the XAUDIO2_END_OF_STREAM flag on the last buffer).
			void STDMETHODCALLTYPE OnStreamEnd() {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnStreamEnd()"));
			}

			// Called when this voice is about to start processing a new buffer.
			void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnBufferStart(%p)"), pBufferContext);
			}

			// Called when this voice has just finished processing a buffer.
			// The buffer can now be reused or destroyed.
			void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnBufferEnd(%p)"), pBufferContext);
			}

			// Called when this voice has just reached the end position of a loop.
			void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnLoopEnd(%p)"), pBufferContext);
			}

			// Called in the event of a critical error during voice processing,
			// such as a failing xAPO or an error from the hardware XMA decoder.
			// The voice may have to be destroyed and re-created to recover from
			// the error.  The callback arguments report which buffer was being
			// processed when the error occurred, and its HRESULT code.
			void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) {
				//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceError(%p, %p)"), pBufferContext, Error);
			}
		};
		
		XAudioBuffer(XAudioEngine *pAudio, const WaveFormat &format, ICallback *pCallback);
		
		~XAudioBuffer();
		
		
		bool Initialize();
		
		void Play();
		
		void Stop();
		
		void Submit(const float *data, svar length);

		float GetVolume();
		void SetVolume(float volume);
		void GetChannelVolumes(int channels, float* volumes);
		void SetChannelVolumes(int channels, const float* volumes);

	private:
		XAudioEngine *m_pAudio;
		CXAudio2VoiceCallback m_voiceCallback;
		IXAudio2SourceVoice *m_pSourceVoice;
		XAUDIO2_BUFFER m_xabuffer;
	};

} }