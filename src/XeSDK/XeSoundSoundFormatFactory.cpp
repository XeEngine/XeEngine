#include "pch.h"
#include <XeSDK/XeSoundSoundFormatFactory.h>
#include <XeSDK/ISoundSoundFormatFactory.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Sound {
	std::list<ISoundFormatFactory*> m_SoundFormatFactoryList;

	void RegisterSoundFormatFactory(ISoundFormatFactory* pSoundFormatFactory)
	{
		m_SoundFormatFactoryList.push_back(pSoundFormatFactory);
	}

	std::list<ISoundFormatFactory*> GetSoundFormatFactories()
	{
		return m_SoundFormatFactoryList;
	}

	bool GetFirstSoundFormatFactory(ISoundFormatFactory& soundFormatFactory, ctstring extension)
	{
		for (auto it = m_SoundFormatFactoryList.begin(); it != m_SoundFormatFactoryList.end(); ++it)
		{
			auto currentSoundFormatFactory = *it;
			if (Xe::String::CompareInsensitive(extension, currentSoundFormatFactory->GetExtension()) == 0)
			{
				soundFormatFactory = *currentSoundFormatFactory;
				return true;
			}
		}
		
		return false;
	}
} }