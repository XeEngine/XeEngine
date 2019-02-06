#include "pch.h"
#include <XeGame/XeGameToolsTilemapDebugger.h>
#include <XeGame/XeGameTilemap2d.h>
#include <imgui/imgui.h>

using namespace Xe;
using namespace Xe::Graphics;
using namespace Xe::Game;

namespace Xe { namespace Game { namespace Tools {
	TilemapDebugger::TilemapDebugger(
		ObjPtr<Xe::Graphics::IContext> context,
		ObjPtr<Xe::Game::ITextureManager> textureManager,
		ObjPtr<Xe::Game::ITilemap2d> tilemap) :
		m_Context(context),
		m_TextureManager(textureManager),
		m_Tilemap(tilemap),
		m_IsVisible(true)
	{ }
	
	TilemapDebugger::~TilemapDebugger()
	{ }

	const char* TilemapDebugger::GetTitle() const
	{
		return "Tilemap";
	}

	bool TilemapDebugger::IsVisible() const
	{
		return m_IsVisible;
	}

	void TilemapDebugger::SetVisible(bool visible)
	{
		m_IsVisible = visible;
	}

	void TilemapDebugger::operator()(const ImGuiFrameArgs& args)
	{
		if (!m_IsVisible) return;

		if (ImGui::Begin(GetTitle()))
		{
			Show(*m_Tilemap);
		}

		ImGui::End();
	}

	void TilemapDebugger::Show(Xe::Game::ITilemap2d& tilemap)
	{
		CtrlBackgroundColor(tilemap);
		CtrlCameraSize(tilemap);
		CtrlCameraPosition(tilemap);
		CtrlTileSize(tilemap);
		CtrlLayers(tilemap);
	}

	void TilemapDebugger::CtrlBackgroundColor(Xe::Game::ITilemap2d& tilemap)
	{
		auto value = tilemap.GetBackgroundColor();
		float v[4]{ value.b, value.g, value.r, value.a };
		if (ImGui::ColorEdit4("Background", v))
		{
			tilemap.SetBackgroundColor({ v[2], v[1], v[0], v[3] });
		}
	}

	void TilemapDebugger::CtrlCameraSize(Xe::Game::ITilemap2d& tilemap)
	{
		const auto& value = tilemap.GetCameraSize();
		int v[2]{ value.x, value.y };
		if (ImGui::InputInt2("Camera size", v))
		{
			tilemap.SetCameraSize({ v[0], v[1] });
		}
	}

	void TilemapDebugger::CtrlCameraPosition(Xe::Game::ITilemap2d& tilemap)
	{
		const auto& value = tilemap.GetCameraPosition();
		float v[2]{ value.x, value.y };
		if (ImGui::InputFloat2("Camera position", v))
		{
			tilemap.SetCameraPosition({ v[0], v[1] });
		}
	}

	void TilemapDebugger::CtrlTileSize(Xe::Game::ITilemap2d& tilemap)
	{
		const auto& value = tilemap.GetTileSize();
		int v[2]{ value.x, value.y };
		if (ImGui::InputInt2("Tile size", v))
		{
			tilemap.SetTileSize({ v[0], v[1] });
		}
	}

	void TilemapDebugger::CtrlLayers(Xe::Game::ITilemap2d& tilemap)
	{
		struct Func
		{
			static bool Get(void* data, int idx, const char** out_str)
			{
				static const char UnnamedLayer[] = "<no name>";

				auto tilemap = (Xe::Game::ITilemap2d*)data;
				const auto& name = tilemap->GetLayer((size_t)idx)->GetName();
				*out_str = name.IsEmpty() ? UnnamedLayer : name.GetData();

				return true;
			}
		};

		int selection = Math::Min(m_LayerSelection, (int)tilemap.GetLayerCount() - 1);
		ImGui::Combo("Layer", &m_LayerSelection, &Func::Get, &tilemap, tilemap.GetLayerCount());

		if (selection >= 0)
		{
			ImGui::Dummy(ImGui::GetStyle().FramePadding);
			ImGui::SameLine();
			if (ImGui::BeginChild("CtrlLayers", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				Show(*tilemap.GetLayer((size_t)selection));
			}
			ImGui::EndChild();
		}
	}

	void TilemapDebugger::Show(Xe::Game::ITilemapLayer& layer)
	{
		CtrlLayerName(layer);
		CtrlLayerBufferSize(layer);
		CtrlLayerScrollOffset(layer);
		CtrlLayerScrollMultiplier(layer);
		CtrlLayerVisibility(layer);
		CtrlLayerTexture(layer);
		CtrlLayerTilesPerRow(layer);
		CtrlLayerPalette(layer);
		CtrlLayerImage(layer);
	}

	void TilemapDebugger::CtrlLayerName(Xe::Game::ITilemapLayer& layer)
	{
		char buffer[0x100];
		const auto& name = layer.GetName();
		auto length = Math::Min<size_t>(name.GetLength(), lengthof(buffer) - 1);
		
		Xe::Memory::Copy(buffer, name.GetData(), length);
		buffer[length] = '\0';

		if (ImGui::InputText("Name", buffer, lengthof(buffer)))
		{
			layer.SetName(buffer);
		}
	}

	void TilemapDebugger::CtrlLayerBufferSize(Xe::Game::ITilemapLayer& layer)
	{
		const auto& value = layer.GetBufferSize();
		int v[2]{ (int)value.x, (int)value.y };
		if (ImGui::InputInt2("Buffer size", v))
		{
			layer.SetBufferSize({ (u32)v[0], (u32)v[1] });
		}
	}

	void TilemapDebugger::CtrlLayerScrollOffset(Xe::Game::ITilemapLayer& layer)
	{
		const auto& value = layer.GetScrollOffset();
		float v[2]{ value.x, value.y };
		if (ImGui::InputFloat2("Scroll offset", v))
		{
			layer.SetScrollOffset({ v[0], v[1] });
		}
	}

	void TilemapDebugger::CtrlLayerScrollMultiplier(Xe::Game::ITilemapLayer& layer)
	{
		const auto& value = layer.GetScrollMultiplier();
		float v[2]{ value.x, value.y };
		if (ImGui::InputFloat2("Scroll multiplier", v))
		{
			layer.SetScrollMultiplier({ v[0], v[1] });
		}
	}

	void TilemapDebugger::CtrlLayerVisibility(Xe::Game::ITilemapLayer& layer)
	{
		auto value = layer.IsVisible();
		if (ImGui::Checkbox("Visible", &value))
		{
			layer.SetVisible(value);
		}
	}

	void TilemapDebugger::CtrlLayerTexture(Xe::Game::ITilemapLayer& layer)
	{
		auto texId = layer.GetTexture();
		if (texId != Xe::Game::TexInvalid)
		{
			ImGui::Text("Texture %p", texId);
		}
		else
		{
			ImGui::Text("Texture not set");
		}
	}

	void TilemapDebugger::CtrlLayerTilesPerRow(Xe::Game::ITilemapLayer& layer)
	{
		auto value = layer.GetTilesPerRow();
		ImGui::Text("Tiles per row %i", value);
	}

	void TilemapDebugger::CtrlLayerPalette(Xe::Game::ITilemapLayer& layer)
	{
		auto texId = layer.GetPalette();
		if (texId != Xe::Game::ClutInvalid)
		{
			ImGui::Text("Palette %p", texId);
		}
		else
		{
			ImGui::Text("Palette not set");
		}
	}

	void TilemapDebugger::CtrlLayerImage(Xe::Game::ITilemapLayer& layer)
	{
		const auto& layerBufferSize = layer.GetBufferSize();
		const auto& tileSize = m_Tilemap->GetTileSize();
		const Xe::Math::Vector2i textureSize =
		{
			(int)layerBufferSize.x * tileSize.x,
			(int)layerBufferSize.y * tileSize.y
		};

		if (RenderToTexture(textureSize))
		{
			ImVec2 ctrlPos = ImGui::GetCursorScreenPos();
			ImGui::Image((ImTextureID)m_LayerSurface.Get(), ImVec2((float)textureSize.x, (float)textureSize.y));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGuiIO& io = ImGui::GetIO();
				ShowLayerDataTooltip(layer, io.MousePos.x - ctrlPos.x, io.MousePos.y - ctrlPos.y);
				ImGui::EndTooltip();
			}
		}
	}

	void TilemapDebugger::ShowLayerDataTooltip(
		Xe::Game::ITilemapLayer& layer, 
		float posX,
		float posY)
	{
		const auto& tileSize = m_Tilemap->GetTileSize();
		const auto& layerSurfaceSize = m_LayerSurface->GetSize();
		int tileX = (int)posX / tileSize.x;
		int tileY = (int)posY / tileSize.y;
		float uvx = tileX * tileSize.x;
		float uvy = tileY * tileSize.y;

		float zoom = 2.0f;
		int maxTileSize = Xe::Math::Max(tileSize.x, tileSize.y);
		if (maxTileSize < 32)
		{
			zoom *= 2.0f;
			if (maxTileSize < 16)
			{
				zoom *= 2.0f;
				if (maxTileSize < 8)
				{
					zoom *= 2.0f;
				}
			}
		}

		Xe::Game::TilemapData layerData;

		ImGui::Image((ImTextureID)m_LayerSurface.Get(),
			ImVec2(tileSize.x * zoom, tileSize.y * zoom),
			ImVec2(uvx / layerSurfaceSize.x, uvy / layerSurfaceSize.y),
			ImVec2((uvx + tileSize.x) / layerSurfaceSize.x, (uvy + tileSize.y) / layerSurfaceSize.y),
			ImColor(255, 255, 255, 255),
			ImColor(255, 255, 255, 128));

		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Text("Position %3i, %3i", tileX, tileY);
		if (layer.Lock(layerData, Xe::Game::Lock_Read))
		{
			const auto tile = layerData.Tile(tileX, tileY);
			ImGui::Text("Raw      %08X", tile.Data);
			ImGui::Text("Index    %04X", tile.Tile);
			ImGui::Text("Mirror   %c", tile.Mirror ? 'Y' : 'N');
			ImGui::Text("Flip     %c", tile.Flip ? 'Y' : 'N');
			ImGui::Text("Rotate   %c", tile.Rotate ? 'Y' : 'N');

			layer.Unlock();
		}
		else
		{
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Unable to lock the layer");
		}

		ImGui::EndGroup();

	}

	bool TilemapDebugger::RenderToTexture(Math::Vector2i size)
	{
		if (m_LayerSurface == nullptr || m_LayerSurface->GetSize() != size)
		{
			m_LayerSurface = nullptr;
			LOGFA(m_Context->CreateSurface(&m_LayerSurface, SurfaceType_Target, size, Color::Format_BGRA8888));
		}

		if (m_LayerSurface == nullptr)
			return false;

		if (m_Drawing == nullptr)
		{
			Xe::Game::Factory(&m_Drawing, m_Context);
		}

		ISurface* pPreviousRenderingSurface;
		m_Context->GetRenderingSurface(&pPreviousRenderingSurface);
		m_Context->SetRenderingSurface(m_LayerSurface);

		Xe::Math::Vector2f pos[]
		{
			{ -1.0f, +1.0f },
			{ +1.0f, -1.0f }
		};

		Xe::Math::Vector2f uv[]
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
		};

		m_Drawing->DrawRectangle(pos, Color::Fuchsia);
		m_Drawing->Flush();

		RenderLayer(0, size);

		m_Context->SetRenderingSurface(pPreviousRenderingSurface);

		return true;
	}

	void TilemapDebugger::RenderLayer(size_t layerIndex, const Math::Vector2i& size)
	{
		Xe::Game::TilemapDrawArgs args;
		Xe::Game::TilemapDrawFlags drawFlags = Xe::Game::TilemapDraw_IgnoreVisibility;

		const auto prevCameraSize = m_Tilemap->GetCameraSize();
		m_Tilemap->SetCameraSize(size);
		m_Tilemap->DrawLayer(layerIndex, args, drawFlags);
		m_Tilemap->SetCameraSize(prevCameraSize);

		Xe::Math::Rectanglef rect =
			Xe::Math::Rectanglef::FromSize({ 0.0f, 0.0f }, { (float)size.x, (float)size.y });
		for (auto drawList : args.Draws)
		{
			Draw(
				drawList,
				m_Context.Get(),
				m_TextureManager.Get(),
				rect);
		}

		RenderLayer(layerIndex, { 0.0f, 0.0f, (float)size.x, (float)size.y });
	}

	void TilemapDebugger::RenderLayer(size_t layerIndex, const Math::Rectanglef& rectangle)
	{
	}

} } }