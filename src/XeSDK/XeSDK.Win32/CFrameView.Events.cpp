#include "pch.h"
#include "CFrameView.h"

#include <Windows.h>
#include <windowsx.h>
#include <dbt.h>

using namespace Xe;
using namespace Xe::Core;

LRESULT CALLBACK CFrameView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		DEV_BROADCAST_DEVICEINTERFACE NotificationFilter = { 0 };
		NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		NotificationFilter.dbcc_classguid =
		//{ 0x25dbce51, 0x6c8f, 0x4a72, 0x8a, 0x6d, 0xb5, 0x4c, 0x2b, 0x4f, 0xc8, 0x35 }; // USB
		{ 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 }; // HID

		m_DevNotify = RegisterDeviceNotification(
			m_hWnd, &NotificationFilter,
			DEVICE_NOTIFY_WINDOW_HANDLE
		);
	} break;
	case WM_NCDESTROY:
		m_isClosed = true;
		UnregisterDeviceNotification(m_DevNotify);
		break;
	case WM_SIZE:
		switch (wParam) {
		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
		case SIZE_MAXSHOW:
		case SIZE_MAXHIDE:
			if (m_isWindowMinimized) {
				m_isWindowMinimized = false;
				m_pFrameHandler->OnVisibilityChanged(true);
			}
			m_size.x = LOWORD(lParam);
			m_size.y = HIWORD(lParam);
			m_pointerEvent.CurrentPointer.Device.Rectangle.left = 0;
			m_pointerEvent.CurrentPointer.Device.Rectangle.top = 0;
			m_pointerEvent.CurrentPointer.Device.Rectangle.right = m_size.x;
			m_pointerEvent.CurrentPointer.Device.Rectangle.bottom = m_size.y;
			m_pFrameHandler->OnSizeChanged(GetSize());
			break;
		case SIZE_MINIMIZED:
			m_isWindowMinimized = true;
			m_pFrameHandler->OnVisibilityChanged(false);
			break;
		}
		break;
	case WM_SETFOCUS:
		m_pFrameHandler->OnFocusGot();
		break;
	case WM_KILLFOCUS:
		m_pFrameHandler->OnFocusLost();
		break;
	case WM_PAINT:
		break;
	case WM_CLOSE:
		if (m_pFrameHandler->OnClosing(false))
			DestroyWindow(hWnd);
		return 0;
	case WM_ERASEBKGND:
		return 0;
	case WM_KEYDOWN:
		OnKey(wParam, lParam, false);
		break;
	case WM_KEYUP:
		OnKey(wParam, lParam, true);
		break;
	case WM_CHAR:
		OnChar(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
		OnMouseButton(wParam, lParam, false);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
		OnMouseButton(wParam, lParam, true);
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(wParam, lParam, false);
		break;
	case WM_MOUSEHWHEEL:
		OnMouseWheel(wParam, lParam, true);
		break;
	case WM_DEVICECHANGE:
		OnDeviceChange(wParam, lParam);
		break;
	case WM_TOUCH:
		OnTouch(hWnd, wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CFrameView::OnChar(WPARAM wParam, LPARAM lParam)
{
	IO::CharacterEvent e;
	e.ScanCode = (u32)((wParam >> 16) & 8);
	e.Character = (u32)wParam;
	m_pKeyboardHandler->OnCharacter(e);
}

void CFrameView::OnKey(WPARAM wParam, LPARAM lParam, bool isReleased)
{
	union Param {
		LPARAM Data;
		struct {
			u32 RepeatCount : 16;
			u32 ScanCode : 8;
			u32 IsExtended : 1;
			u32 Reserved : 4;
			u32 ContextCode : 1;
			u32 PreviousState : 1;
			u32 TransitionState : 1;
		};
	};

	IO::KeyboardEvent e;
	Param param; param.Data = lParam;
	e.ScanCode = param.ScanCode;
	e.VirtualCode = (u32)wParam;
	isReleased ? m_pKeyboardHandler->OnKeyReleased(e) :
		m_pKeyboardHandler->OnKeyPressed(e);
}

void CFrameView::OnMouseWheel(WPARAM wParam, LPARAM lParam, bool isHorizontal)
{
	float value = (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	if (isHorizontal) m_pPointerHandler->OnPointerWheel(Math::Vector2f(value, 0.0f));
	else m_pPointerHandler->OnPointerWheel(Xe::Math::Vector2f(0.0f, value));
}

void CFrameView::OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
#if (WINVER >= 0x0601)
	UINT cInputs = LOWORD(wParam);
	PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
	if (pInputs != NULL)
	{
		if (GetTouchInputInfo((HTOUCHINPUT)lParam,
			cInputs, pInputs, sizeof(TOUCHINPUT)))
		{
			UINT type = 0;
			RECT rect;
			GetClientRect(m_hWnd, &rect);
			/*UINT count = Math::Min<UINT>(cInputs, pointerEvent.MaxPointersCount - 1);
			for (UINT i = 0; i < count; i++) {
				TOUCHINPUT& ti = pInputs[i];
				IO::Pointer& p = pointerEvent.Pointer[i + 1];
				if (p.IsProcessed == false) {
					p.IsProcessed = true;
					p.Time = Timer::Current();
				}
				POINT touchPoint;
				touchPoint.x = TOUCH_COORD_TO_PIXEL(ti.x);
				touchPoint.y = TOUCH_COORD_TO_PIXEL(ti.y);
				ScreenToClient(m_hWnd, &touchPoint);

				p.Device.Type = IO::PointerType_Touch;
				p.Device.Rectangle.left = rect.left;
				p.Device.Rectangle.top = rect.top;
				p.Device.Rectangle.right = rect.right;
				p.Device.Rectangle.bottom = rect.bottom;
				p.Device.Is3dSupported = false;
				if (ti.dwFlags & TOUCHEVENTF_MOVE) {
					p.Position.x = (svar)touchPoint.x;
					p.Position.y = (svar)touchPoint.y;
					p.Buttons = 0;
					p.Pressure = 1.0f;
					pointerEvent.PointerIndex = i + 1;
					type = 1;
				}
				else if (ti.dwFlags & TOUCHEVENTF_DOWN) {
					p.Position.x = (svar)touchPoint.x;
					p.Position.y = (svar)touchPoint.y;
					p.Buttons = 0;
					p.Pressure = 1.0f;
					pointerEvent.PointerIndex = i + 1;
					type = 2;
				}
				else if (ti.dwFlags & TOUCHEVENTF_UP) {
					p.Position.x = -1;
					p.Position.y = -1;
					p.Buttons = 0;
					p.Pressure = 0.0f;
					pointerEvent.PointerIndex = i + 1;
					type = 3;
				}
			}
			for (UINT i = count; i < pointerEvent.MaxPointersCount; i++)
				pointerEvent.Pointer[i].IsProcessed = false;*/
			if (!CloseTouchInputHandle((HTOUCHINPUT)lParam))
			{
				// error handling
			}
			switch (type) {
			case 1: //m_pCallback->OnPointerMove(pointerEvent);
				break;
			case 2: //m_pCallback->OnPointerPressed(pointerEvent);
				break;
			case 3: //m_pCallback->OnPointerReleased(pointerEvent);
				break;
			}
		}
		else
		{
			// GetLastError() and error handling
		}
		delete[] pInputs;
	}
	else
	{
		// error handling, presumably out of memory
	}
#endif
}
uvar CFrameView::GetMouseButtonFromWParam(WPARAM wParam)
{
	return ((wParam & MK_LBUTTON) ? PointerButton_Left : 0) |
		((wParam & MK_RBUTTON) ? PointerButton_Right : 0) |
		((wParam & MK_MBUTTON) ? PointerButton_Center : 0) |
		((wParam & MK_XBUTTON1) ? PointerButton_XButton1 : 0) |
		((wParam & MK_XBUTTON2) ? PointerButton_XButton2 : 0);
}
void CFrameView::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	m_pointerEvent.CurrentPointer.Time = Timer();
	m_pointerEvent.CurrentPointer.Position.x = GET_X_LPARAM(lParam);
	m_pointerEvent.CurrentPointer.Position.y = GET_Y_LPARAM(lParam);
	if (wParam != 0)
	{
		m_pointerEvent.CurrentPointer.Buttons = GetMouseButtonFromWParam(wParam);
		m_pointerEvent.CurrentPointer.Pressure = 1.0f;
		m_pointerEvent.CurrentPointer.Action = PointerAction_Moved;
	}
	else
	{
		m_pointerEvent.CurrentPointer.Buttons = 0;
		m_pointerEvent.CurrentPointer.Pressure = 0.0f;
		m_pointerEvent.CurrentPointer.Action = PointerAction_Moved;
	}

	m_pPointerHandler->OnPointerMoved(m_pointerEvent);
}

void CFrameView::OnMouseButton(WPARAM wParam, LPARAM lParam, bool isReleased)
{
	m_pointerEvent.CurrentPointer.Time = Timer();
	m_pointerEvent.CurrentPointer.Position.x = GET_X_LPARAM(lParam);
	m_pointerEvent.CurrentPointer.Position.y = GET_Y_LPARAM(lParam);
	if (wParam != 0)
	{
		m_pointerEvent.CurrentPointer.Buttons = GetMouseButtonFromWParam(wParam);
		m_pointerEvent.CurrentPointer.Pressure = 1.0f;
	}
	else
	{
		m_pointerEvent.CurrentPointer.Buttons = 0;
		m_pointerEvent.CurrentPointer.Pressure = 0.0f;
	}

	if (isReleased)
	{
		m_pointerEvent.CurrentPointer.Action = PointerAction_Released;
		m_pPointerHandler->OnPointerReleased(m_pointerEvent);
	}
	else
	{
		m_pointerEvent.CurrentPointer.Action = PointerAction_Pressed;
		m_pPointerHandler->OnPointerPressed(m_pointerEvent);
	}
}

void CFrameView::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	Xe::Core::DeviceArgs args;
	switch (wParam)
	{
	case DBT_DEVNODES_CHANGED:
		// A device has been added to or removed from the system.
		m_pApplicationHandler->OnDevice(args);
		break;
	case DBT_DEVICEARRIVAL:
		// Check whether a CD or DVD was inserted into a drive.
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		// Check whether a CD or DVD was removed from a drive.
		break;
	}
}