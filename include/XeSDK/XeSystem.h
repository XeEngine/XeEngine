#pragma once

namespace Xe {
	class System {
		//! \brief used to describe the version of the engine or its sub-components
		struct Version {
			//! \brief the higher, and more valuable, number that represent the current version
			u16 high;
			//! \brief low version
			u16 low;
			//! \brief lower version value to describe the version of the product
			u32 build;
		};
		
		//! \brief get the version of current XeEngine library
		//! \return version of the product
		/** \details Return value can not represent the real current build of
		 * the product. In fact, even Version.build, represent the minimum
		 * value supported to the product. It does not increment every build.
		 * In order to have an unique way to enumerate the builds, please
		 * use GetBuildDate.
		 */
		static const Version& GetVersion();

		//! \brief get the build date of current XeEngine library
		static ctstring GetBuildDate();
	};
}