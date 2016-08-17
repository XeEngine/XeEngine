#pragma once
#include <XeSDK/XeCollectionsArray.h>
#include <XeSDK/XeString.h>

namespace Xe {
	namespace Core {
		//! \brief This manage the life-cycle of a XeEngine application.
		class IApplication {
		public:
			//! \brief Get current application.
			//! \details This value is never changed through the app.
			static IApplication* Get();

			//! \brief This is the entry point of an application.
			virtual void OnInitialize(Collections::Array<String*> Args) = 0;

			//! \brief Where the logic side of the application lives.
			//! \details Called after OnInitialize and before OnDestroy.
			virtual void OnRun() = 0;

			virtual void OnSuspend() = 0;

			virtual void OnResume() = 0;

			virtual void OnBackground() = 0;



			//! \brief Inverse the OnInitialize
			virtual void OnDestroy() = 0;
		};
	}
}