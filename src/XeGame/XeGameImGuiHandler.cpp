#include "pch.h"
#include <XeSDK/ICoreView.h>
#include <XeGame/XeGameImGuiHandler.h>
#include <XeGame/IGameDrawing2d.h>
#include <imgui/imgui.h>

using namespace Xe::Game;
using namespace ImGui;

#define VK_BACK		0x08
#define VK_TAB		0x09
#define VK_RETURN	0x0D
#define VK_ESCAPE	0x1B
#define VK_SPACE	0x20
#define VK_PRIOR	0x21
#define VK_NEXT		0x22
#define VK_END		0x23
#define VK_HOME		0x24
#define VK_LEFT		0x25
#define VK_UP		0x26
#define VK_RIGHT	0x27
#define VK_DOWN		0x28
#define VK_INSERT	0x2D
#define VK_DELETE	0x2E


ImGuiHandler::ImGuiHandler(Xe::Graphics::IContext* pContext) :
	m_Context(pContext),
	m_ImGuiContext(nullptr)
{

}

bool ImGuiHandler::OnInitialize()
{
	m_ImGuiContext = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

	io.ConfigFlags =
		ImGuiConfigFlags_NavEnableGamepad |
		ImGuiConfigFlags_NavEnableSetMousePos;

	io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Space] = VK_SPACE;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	ImGui::StyleColorsDark();

	// Build texture atlas
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	Xe::Graphics::DataDesc desc;
	desc.data = pixels;
	desc.pitch = width * 4;

	if (!(m_Context->CreateSurface(
		&m_ImGuiFonts,
		Xe::Graphics::SurfaceType_Texture,
		Xe::Graphics::Size(width, height),
		Xe::Graphics::Color::Format_BGRA8888,
		desc)))
	{
		LOGE("Unable to create the ImGui font surface");
		return false;
	}

	ImGui::GetIO().Fonts->TexID = *&m_ImGuiFonts;

	return true;
}

void ImGuiHandler::OnDestroy()
{
	if (m_ImGuiContext)
	{
		ImGui::DestroyContext(m_ImGuiContext);
		m_ImGuiContext = nullptr;
	}
}

void ImGuiHandler::OnSuspend()
{

}

void ImGuiHandler::OnResume()
{

}

void ImGuiHandler::OnRun()
{

}

void ImGuiHandler::OnDraw()
{
	ImGui::NewFrame();
	(*this)(ImGuiFrameArgs());
	ImGui::Render();
	Render(ImGui::GetDrawData());
}

void ImGuiHandler::OnDevice(const Xe::Core::DeviceArgs& args)
{

}


bool ImGuiHandler::OnAttach(Xe::Core::IFrameView* pFrameView)
{
	return true;
}

bool ImGuiHandler::OnClosing(bool forced)
{
	return true;
}

void ImGuiHandler::OnFocusGot()
{

}

void ImGuiHandler::OnFocusLost()
{
	InputReset();
}

void ImGuiHandler::OnVisibilityChanged(bool visibility)
{

}

void ImGuiHandler::OnSizeChanged(const Xe::Graphics::Size& size)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = (float)size.x;
	io.DisplaySize.y = (float)size.y;
}

void ImGuiHandler::OnOrientationChanged(Xe::Graphics::Orientation orientation)
{

}

void ImGuiHandler::OnDpiChanged(float dpi)
{

}


void ImGuiHandler::OnCharacter(const Xe::IO::CharacterEvent& e)
{
	ImGui::GetIO().AddInputCharacter(e.Character);
}

void ImGuiHandler::OnKeyPressed(const Xe::IO::KeyboardEvent& e)
{
	ImGui::GetIO().KeysDown[e.VirtualCode] = true;
}

void ImGuiHandler::OnKeyReleased(const Xe::IO::KeyboardEvent& e)
{
	ImGui::GetIO().KeysDown[e.VirtualCode] = false;
}


void ImGuiHandler::OnPointerMoved(const Xe::IO::PointerEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantSetMousePos)
	{
		LOGW("ImGuiIO::WantSetMousePos not implemented");
	}

	io.MousePos.x = (float)e.CurrentPointer.Position.x;
	io.MousePos.y = (float)e.CurrentPointer.Position.y;
}

void ImGuiHandler::OnPointerPressed(const Xe::IO::PointerEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Left);
	io.MouseDown[1] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Right);
	io.MouseDown[2] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Center);
	io.MouseDown[3] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_XButton1);
	io.MouseDown[4] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_XButton2);
}

void ImGuiHandler::OnPointerReleased(const Xe::IO::PointerEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Left);
	io.MouseDown[1] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Right);
	io.MouseDown[2] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_Center);
	io.MouseDown[3] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_XButton1);
	io.MouseDown[4] = !!(e.CurrentPointer.Buttons & Xe::IO::PointerButton_XButton2);
}

void ImGuiHandler::OnPointerEnter(const Xe::IO::PointerEvent& e)
{

}

void ImGuiHandler::OnPointerLeave(const Xe::IO::PointerEvent& e)
{
	InputReset();
}

void ImGuiHandler::OnPointerWheel(const Xe::Math::Vector2<float>& wheel)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH = wheel.x;
	io.MouseWheel = wheel.y;
}

void ImGuiHandler::InputReset()
{
	ImGuiIO& io = ImGui::GetIO();
	Xe::Memory::Fill(io.KeysDown, 0, sizeof(io.KeysDown));
	Xe::Memory::Fill(io.MouseDown, 0, sizeof(io.MouseDown));
	io.MouseWheel = io.MouseWheelH = 0;
}

void ImGuiHandler::Render(ImDrawData* draw_data)
{
	if (draw_data == nullptr || draw_data->TotalVtxCount == 0) return;

	float mx = 2.0f / draw_data->DisplaySize.x;
	float my = 2.0f / draw_data->DisplaySize.y;

	auto vertexData = new Xe::Graphics::Vertex[draw_data->TotalVtxCount];
	auto indexData = new ImDrawIdx[draw_data->TotalIdxCount];

	auto vDst = vertexData;
	auto iDst = indexData;
	ImDrawIdx idx = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const auto& cmd_list = *draw_data->CmdLists[n];
		auto vSrc = cmd_list.VtxBuffer.Data;
		for (int i = 0; i < cmd_list.VtxBuffer.Size; i++)
		{
			vDst->Position.x = vSrc->pos.x * mx - 1.0f;
			vDst->Position.y = 1.0f - vSrc->pos.y * my;
			vDst->Texture.x = vSrc->uv.x;
			vDst->Texture.y = vSrc->uv.y;
			vDst->Texture.z = Xe::Game::IDrawing2d::MODE_TEXTURED;
			vDst->Color = Xe::Graphics::Color(vSrc->col);
			++vSrc, ++vDst;
		}

		Xe::Memory::Copy(iDst, cmd_list.IdxBuffer.Data, cmd_list.IdxBuffer.Size * sizeof(u16));
		for (int i = 0; i < cmd_list.IdxBuffer.Size; i++)
		{
			iDst[i] += idx;
		}

		iDst += cmd_list.IdxBuffer.Size;
		idx += cmd_list.VtxBuffer.Size;
	}
	ASSERT(vDst == vertexData + draw_data->TotalVtxCount);
	ASSERT(iDst == indexData + draw_data->TotalIdxCount);

	Xe::Graphics::IBuffer *vertexBuffer, *indexBuffer;

	Xe::Graphics::BufferDesc vbDesc;
	vbDesc.Usage = Xe::Graphics::Usage_Static;
	vbDesc.Type = Xe::Graphics::BufferType_Vertex;
	vbDesc.Length = draw_data->TotalVtxCount * sizeof(Xe::Graphics::Vertex);
	Xe::Graphics::DataDesc vbDataDesc;
	vbDataDesc.data = vertexData;
	vbDataDesc.pitch = vbDesc.Length;
	LOGA(m_Context->CreateBuffer(&vertexBuffer, vbDesc, &vbDataDesc));
	delete[] vertexData;

	Xe::Graphics::BufferDesc ibDesc;
	ibDesc.Usage = Xe::Graphics::Usage_Static;
	ibDesc.Type = Xe::Graphics::BufferType_Index16;
	ibDesc.Length = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
	Xe::Graphics::DataDesc ibDataDesc;
	ibDataDesc.data = indexData;
	ibDataDesc.pitch = ibDesc.Length;
	LOGA(m_Context->CreateBuffer(&indexBuffer, ibDesc, &ibDataDesc));
	delete[] indexData;

	auto isScissorEnabled = m_Context->IsScissorEnabled();
	m_Context->SetScissorEnabled(true);
	u32 primitiveOffset = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const auto& cmd_list = *draw_data->CmdLists[n];
		for (int i = 0; i < cmd_list.CmdBuffer.Size; i++)
		{
			const auto& pcmd = cmd_list.CmdBuffer[i];

			auto surface = static_cast<Xe::Graphics::ISurface*>(pcmd.TextureId);
			m_Context->SelectSurface(surface, 0);
			m_Context->SetVertexBuffer(vertexBuffer);
			m_Context->SetIndexBuffer(indexBuffer);
			m_Context->SetScissor({ (int)pcmd.ClipRect.x, (int)pcmd.ClipRect.y, (int)pcmd.ClipRect.z, (int)pcmd.ClipRect.w });

			m_Context->DrawIndexed(Xe::Graphics::Primitive_TriangleList, pcmd.ElemCount, primitiveOffset);
			primitiveOffset += pcmd.ElemCount;
		}
	}
	m_Context->SetVertexBuffer(nullptr);
	m_Context->SetIndexBuffer(nullptr);
	m_Context->SetScissorEnabled(isScissorEnabled);

	indexBuffer->Release();
	vertexBuffer->Release();
}