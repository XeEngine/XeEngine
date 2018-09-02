#include <XeSDK/IObject.h>
#include <list>

namespace Xe { namespace Sound {
	interface IAudioSource;
	interface ISoundFormatFactory;

	//! \brief Register a sound format factory.
	void RegisterSoundFormatFactory(ISoundFormatFactory* pSoundFormatFactory);

	//! \brief Get a list of registered ISoundFormatFactory.
	std::list<ISoundFormatFactory*> GetSoundFormatFactories();

	//! \brief Get the first sound format factory that respects the given extension.
	bool GetFirstSoundFormatFactory(ISoundFormatFactory& soundFormatFactory, ctstring extension);
} }