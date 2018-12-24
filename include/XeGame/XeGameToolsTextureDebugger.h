#pragma once
#include <XeGame/IGameToolsFrame.h>
#include <XeGame/ITextureManager.h>

namespace Xe { namespace Game { namespace Tools
{
	class TextureDebugger :	public Xe::Game::Tools::IFrame
	{
		Xe::ObjPtr<Xe::Game::ITextureManager> m_TextureManager;
		Xe::Game::TextureManagerProfileEntry m_TextureEntries[0x100];
		bool m_IsVisible;
		int m_SelectedTexture;

		void Show(const Xe::Game::TextureManagerProfile& profile);
		void Show(const Xe::Game::TextureManagerProfileEntry& entry);
	public:
		TextureDebugger(Xe::Game::ITextureManager* pTextureManager);
		virtual ~TextureDebugger();

		const char* GetTitle() const;
		bool IsVisible() const;
		void SetVisible(bool visible);

		void operator()(const Xe::Game::ImGuiFrameArgs& args);
	};
} } }