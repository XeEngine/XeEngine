#pragma once
#include <XeGame/IGamePlatformCallback.h>
#include <XeSDK/IObject.h>

namespace Xe { namespace Game {
	struct GamePlatformInit
	{
		u32 AppId;
		IGamePlatformCallback* Callback;
	};

	interface IGamePlatform : public IObject
	{
		virtual void Update(double deltaTime) = 0;

		virtual bool IsOverlayActivated() const = 0;
	};

	bool GamePlatformFactory(IGamePlatform** ppGamePlatform, const GamePlatformInit& init);
} }