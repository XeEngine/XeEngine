#include "pch.h"
#include "GamePlatformConfig.h"

#if GAMEPLATFORM == GAMEPLATFORM_DUMMY

#include <XeGame/IGamePlatform.h>

namespace Xe { namespace Game {
	struct DummyPlatform : IGamePlatform
	{
		IGamePlatformCallback* m_pCallback;

		DummyPlatform()
		{

		}

		~DummyPlatform()
		{
		}

		bool Initialize(const GamePlatformInit& init)
		{
			if (init.Callback == nullptr)
			{
				Xe::Logger::DebugError("IGamePlatformCallback cannot be null.");
				return false;
			}

			m_pCallback = init.Callback;
			return true;
		}

		void Update(double deltaTime)
		{
		}

		bool IsOverlayActivated() const
		{
			return false;
		}
	};

	bool GamePlatformFactory(IGamePlatform** ppGamePlatform, const GamePlatformInit& init)
	{
		auto pGamePlatform = new DummyPlatform;
		if (pGamePlatform->Initialize(init))
		{
			*ppGamePlatform = pGamePlatform;
			return true;
		}

		*ppGamePlatform = nullptr;
		delete pGamePlatform;
		return false;
	}
} }

#endif