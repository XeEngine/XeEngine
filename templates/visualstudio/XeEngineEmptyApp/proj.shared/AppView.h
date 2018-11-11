#pragma once
#include <XeSDK.h>

namespace $ext_safeprojectname$ {
	class $ext_safeprojectname$View :
		public Xe::Core::IApplicationHandler,
		public Xe::Core::IFrameHandler,
		public Xe::Core::IKeyboardHandler,
		public Xe::Core::IPointerHandler
	{
		Xe::Core::IFrameView* m_pFrameView;
		Xe::Graphics::IContext* m_pContext;

		bool m_IsVblankEnabled;
		bool m_IsFullscreen;
		bool m_IsOverlayActivated;

		void Draw();
		void SwapBuffer(bool isVblankEnabled);
	public:
		$ext_safeprojectname$View();
		~$ext_safeprojectname$View();

		bool OnInitialize();
		void OnDestroy();
		void OnRun();
		void OnDraw();
		void OnSuspend();
		void OnResume();
		void OnDevice(const Xe::Core::DeviceArgs& args);

		bool OnAttach(Xe::Core::IFrameView* pFrameView);
		bool OnClosing(bool forced);
		void OnFocusGot();
		void OnFocusLost();
		void OnVisibilityChanged(bool visible);
		void OnSizeChanged(const Xe::Graphics::Size& size);
		void OnOrientationChanged(Xe::Core::Orientation orientation);
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

		void OnGameOverlayActivated(bool activated);
	};
}