#pragma once
#include <XeSDK/IDelegate.h>
#include <XeGame/XeGameImGuiHandler.h>

namespace Xe { namespace Game { namespace Tools
{
	interface IFrame : public Xe::IDelegate<const Xe::Game::ImGuiFrameArgs&>
	{
		virtual const char* GetTitle() const = 0;

		virtual bool IsVisible() const = 0;
		
		virtual void SetVisible(bool visible) = 0;
	};
} } }