#include "pch.h"
#include "GamePlatformConfig.h"

#if GAMEPLATFORM == GAMEPLATFORM_STEAM

#include <XeGame/IGamePlatform.h>
#include <steam/steam_api.h>

namespace Xe {
	namespace Game {
		struct SteamPlatformCallback
		{
			IGamePlatformCallback* m_pCallback;

			SteamPlatformCallback(IGamePlatformCallback* pCallback) : m_pCallback(pCallback) {}

			STEAM_CALLBACK(SteamPlatformCallback, OnGameOverlayActivated, GameOverlayActivated_t);
		};

		struct SteamPlatform : IGamePlatform
		{
			IGamePlatformCallback* m_pCallback;
			SteamPlatformCallback* m_SteamCallback;

			SteamPlatform()
			{

			}

			~SteamPlatform()
			{
				SteamAPI_Shutdown();
				delete m_SteamCallback;
			}

			bool Initialize(const GamePlatformInit& init)
			{
				if (init.Callback == nullptr)
				{
					LOGE("IGamePlatformCallback cannot be null.");
					return false;
				}

				m_pCallback = init.Callback;
				m_SteamCallback = new SteamPlatformCallback(m_pCallback);

				// https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RestartAppIfNecessary
				if (SteamAPI_RestartAppIfNecessary(init.AppId))
				{
					exit(0);
				}
				if (SteamAPI_Init())
				{
					LOGD("SteamAPI_Init was called with success.");
					return true;
				}
				else
				{
					LOGE("SteamAPI_Init failed.");
					return false;
				}
			}

			void Update(double deltaTime)
			{
				SteamAPI_RunCallbacks();
			}

			bool IsOverlayActivated() const
			{
				return SteamUtils()->IsOverlayEnabled();
			}
		};

		void SteamPlatformCallback::OnGameOverlayActivated(GameOverlayActivated_t* pCallback)
		{
			m_pCallback->OnGameOverlayActivated(pCallback->m_bActive);
		}

		bool GamePlatformFactory(IGamePlatform** ppGamePlatform, const GamePlatformInit& init)
		{
			auto pGamePlatform = new SteamPlatform;
			if (pGamePlatform->Initialize(init))
			{
				*ppGamePlatform = pGamePlatform;
				return true;
			}

			*ppGamePlatform = nullptr;
			delete pGamePlatform;
			return false;
		}
	}
}

#endif