#pragma once
#include <XeSDK.h>
#include <list>

struct ImDrawData;
struct ImGuiContext;

namespace Xe { namespace Game {
	struct ImGuiFrameArgs
	{
		void* CustomData;
	};

	class ImGuiHandler :
		public Xe::Core::IApplicationHandler,
		public Xe::Core::IFrameHandler,
		public Xe::Core::IKeyboardHandler,
		public Xe::Core::IPointerHandler,
		public Xe::Event<const ImGuiFrameArgs&>
	{
		Xe::ObjPtr<Xe::Graphics::IContext> m_Context;
		Xe::ObjPtr<Xe::Graphics::ISurface> m_ImGuiFonts;
		ImGuiContext* m_ImGuiContext;

		void InputReset();
		void Render(ImDrawData* draw_data);

	public:
		ImGuiHandler(Xe::Graphics::IContext* pContext);

		bool OnInitialize();
		void OnDestroy();
		void OnSuspend();
		void OnResume();
		void OnRun();
		void OnDraw();
		void OnDevice(const Xe::Core::DeviceArgs& args);

		bool OnAttach(Xe::Core::IFrameView* pFrameView);
		bool OnClosing(bool forced);
		void OnFocusGot();
		void OnFocusLost();
		void OnVisibilityChanged(bool visibility);
		void OnSizeChanged(const Xe::Graphics::Size& size);
		void OnOrientationChanged(Xe::Graphics::Orientation orientation);
		void OnDpiChanged(float dpi);

		void OnCharacter(const Xe::IO::CharacterEvent& e);
		void OnKeyPressed(const Xe::IO::KeyboardEvent& e);
		void OnKeyReleased(const Xe::IO::KeyboardEvent& e);

		void OnPointerMoved(const Xe::IO::PointerEvent& e);
		void OnPointerPressed(const Xe::IO::PointerEvent& e);
		void OnPointerReleased(const Xe::IO::PointerEvent& e);
		void OnPointerEnter(const Xe::IO::PointerEvent& e);
		void OnPointerLeave(const Xe::IO::PointerEvent& e);
		void OnPointerWheel(const Xe::Math::Vector2<float>& wheel);
	};
} }