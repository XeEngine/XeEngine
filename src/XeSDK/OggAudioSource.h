#pragma once
#include <XeSDK/XeSoundGenericAudioSource.h>
#include <XeSDK/IIoStream.h>
#include <vorbis/vorbisfile.h>

namespace Xe { namespace Sound {
	class OggAudioSource : public GenericAudioSource
	{
		Xe::IO::IStream* m_pStream;
		OggVorbis_File m_Vorbis;
		int m_CurrentSection;
		bool m_Eof;

		void SetPositionSub(SampleOffset position);
		int ReadSub(void *data, int offset, int length);

		static size_t ReadFunc(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int SeekFunc(void *datasource, ogg_int64_t offset, int whence);
		static long TellFunc(void *datasource);
	public:
		OggAudioSource();
		~OggAudioSource();

		bool Initialize(Xe::IO::IStream* pStream);
	};
} }