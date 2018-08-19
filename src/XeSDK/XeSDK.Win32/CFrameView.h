#pragma once
#include <XeSDK/ICoreView.h>
#include <XeSDK/XeCoreFrameView.h>
#include <XeSDK/XeIOPointer.h>
#include <XeSDK/XeString.h>

class CFrameView : public Xe::Core::IFrameView
{
	static svar m_classesInstanceCount;
	static ATOM m_atom;

	HINSTANCE m_hInstance;
	DWORD m_style;
	DWORD m_styleEx;
	WINDOWPLACEMENT m_OriginalWindowPlacement;
	HWND m_hWnd;
	bool m_isClosed;
	bool m_isFullscreenSwitchEnabled;
	bool m_isWindowMinimized;
	Xe::Graphics::Size m_size;

	Xe::Core::IFrameHandler* m_pFrameHandler;
	Xe::Core::IKeyboardHandler* m_pKeyboardHandler;
	Xe::Core::IPointerHandler* m_pPointerHandler;

	Xe::IO::PointerEvent m_pointerEvent;
	bool m_IsViewInitialized;

	// Inherted from IFrameView
	void SetFrameHandler(Xe::Core::IFrameHandler* pFrameHandler);
	void SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler);
	void SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler);
	bool DispatchEvents(Xe::Core::DispatchType type);
	void SetTitle(const Xe::String& title);
	Xe::Graphics::Size GetSize() const;
	bool SetSize(const Xe::Graphics::Size& size);
	void SetFullScreen(bool fullScreen);
	Xe::Core::Orientation GetOrientation() const;
	void SetPreferredOrientation(Xe::Core::Orientation orientation);
	float GetScale() const;
	void* GetSystemWindow() const;

	// Specific members
	void SetViewInitializeState(bool isInitialized);
	void SetTitleT(const char* title);
	void SetTitleT(const wchar_t* title);

	ATOM RegisterWindowClass();
	void UnregisterWindowClass();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Events
	void OnChar(WPARAM wParam, LPARAM lParam);
	void OnKey(WPARAM wParam, LPARAM lParam, bool isReleased);
	void OnMouseWheel(WPARAM wParam, LPARAM lParam, bool isHorizontal);
	void OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam);
	uvar GetMouseButtonFromWParam(WPARAM wParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnMouseButton(WPARAM wParam, LPARAM lParam, bool isReleased);

public:
	CFrameView();
	~CFrameView();

	bool Initialize(const Xe::Core::FrameViewInitDesc& desc);

	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};