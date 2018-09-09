#pragma once
#include <XeSDK/ISoundAudioSource.h>
#include <XeSDK/IIoStream.h>
#include <vorbis/vorbisfile.h>

namespace Xe { namespace Sound {
	class OggAudioSource : public IAudioSource
	{
		char m_Pcmout[4096];
		Xe::IO::IStream* m_pStream;
		OggVorbis_File m_Vorbis;
		int m_CurrentSection;
		bool m_Eof;

		void SetPositionCurrentSub(SampleOffset position);
		svar ReadSub(void *data, svar offset, SampleOffset count);

		void ReadS8(void* dst, float** src, int samplesCount);
		void ReadS16(void* dst, float** src, int samplesCount);
		void ReadS24(void* dst, float** src, int samplesCount);
		void ReadF32(void* dst, float** src, int samplesCount);

		static size_t ReadFunc(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int SeekFunc(void *datasource, ogg_int64_t offset, int whence);
		static long TellFunc(void *datasource);
	public:
		OggAudioSource();
		~OggAudioSource();

		bool Initialize(Xe::IO::IStream* pStream);
	};
} }