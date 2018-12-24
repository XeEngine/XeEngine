#include "pch.h"
#include <XeGame/XeGameToolsTextureDebugger.h>
#include <imgui/imgui.h>

using namespace Xe::Game;

namespace Xe { namespace Game { namespace Tools {
	TextureDebugger::TextureDebugger(
		Xe::Game::ITextureManager* pTextureManager) :
		m_TextureManager(pTextureManager),
		m_SelectedTexture(0),
		m_IsVisible(true)
	{
		m_TextureManager->AddRef();
	}
	
	TextureDebugger::~TextureDebugger()
	{ }

	const char* TextureDebugger::GetTitle() const
	{
		return "Texture manager";
	}

	bool TextureDebugger::IsVisible() const
	{
		return m_IsVisible;
	}

	void TextureDebugger::SetVisible(bool visible)
	{
		m_IsVisible = visible;
	}

	void TextureDebugger::operator()(const ImGuiFrameArgs& args)
	{
		if (!m_IsVisible) return;

		if (ImGui::Begin(GetTitle()))
		{
			TextureManagerProfile textureProfile;
			if (m_TextureManager->GetProfile(textureProfile))
			{
				Show(textureProfile);
			}
		}

		ImGui::End();
	}

	void TextureDebugger::Show(const TextureManagerProfile& profile)
	{
		ImGui::Text("Textures\nCLUTs\nReferences");
		ImGui::SameLine();
		ImGui::Text("%3i / %3i\n%3i / %3i\n%4i",
			profile.TexturesCount,
			profile.MaxTexturesCount,
			profile.ClutsCount,
			profile.MaxClutsCount,
			profile.TexturesReferencesCount);

		int i = 0;
		const TextureManagerProfileEntry* entry = nullptr;
		for (auto it = profile.Textures.begin();
			it != profile.Textures.end(); ++it, ++i)
		{
			if (m_SelectedTexture == i)
			{
				entry = &*it;
			}
		}

		const char* preview = entry ? entry->Name.c_str() : nullptr;
		if (ImGui::BeginCombo("Textures", preview))
		{
			i = 0;
			for (auto it = profile.Textures.begin();
				it != profile.Textures.end(); ++it, ++i)
			{
				const auto& entry = *it;

				bool isSelected = m_SelectedTexture == i;
				if (ImGui::Selectable(entry.Name.c_str(), &isSelected))
				{
					if (isSelected)
					{
						m_SelectedTexture = i;
					}
				}
			}
			ImGui::EndCombo();
		}

		if (entry)
		{
			Show(*entry);
		}
	}

	void TextureDebugger::Show(const Xe::Game::TextureManagerProfileEntry& entry)
	{
		ImGui::Text("ID\nSize\nRefs\nCLUTs");
		ImGui::SameLine();
		ImGui::Text("%i\n%ix%i\n%i\n%i",
			entry.TexId,
			entry.Width, entry.Height,
			entry.ReferencesCount,
			entry.Cluts.size());

		auto surface = m_TextureManager->GetSurface(entry.TexId);
		const auto& size = surface->GetSize();
		float width = ImGui::GetContentRegionAvailWidth();
		float height = size.y * width / size.x;
		ImGui::Image(surface, ImVec2(width, height));
	}
} } }