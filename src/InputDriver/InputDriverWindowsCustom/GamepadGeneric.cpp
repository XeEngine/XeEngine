#include "pch.h"
#include "GamepadGeneric.h"
#include <XeSDK/XeIODevices.h>

namespace Xe {
	u8 GamepadGeneric::ToU8(float data) {
		return (BYTE)(Math::Min(Math::Max(data, 0.0f), 1.0f) * 255.0f);
	}

	float GamepadGeneric::FromS8(BYTE data) {
		if (data > 0x80) return (float)(data - 0x80) / 127.0f;
		if (data < 0x80) return (float)(data - 0x7F) / 127.0f;
		return 0.0f;
	}

	float GamepadGeneric::FromU8(BYTE data) {
		return (float)data / 255.0f;
	}

	GamepadGeneric::GamepadGeneric(const Xe::IO::HidDevice& hidDevice) :
		m_BufferRead(nullptr),
		m_BufferWrite(nullptr),
		m_Handle(nullptr),
		m_Thread(nullptr),
		m_ThreadId(0),
		m_Closed(false),
		m_IsInvalidated(false)
	{
		LOGI("Opening gamepad at %s\n", hidDevice.Path);
		if (OpenDevice(hidDevice.Path, false)) {
			LOGD("Gamepad connected with success!\n");
		}
		else {
			LOGW("Unable to connect to the gamepad %s.\n", hidDevice.Path);
		}

		if (IsAvailable()) {
			Capabilities.Data = 0x53ffffff;
			Capabilities.Gyroscope = 0;			// 0x04000000
			Capabilities.Accelerometer = 0;		// 0x08000000
			Capabilities.Audio = 0;				// 0x20000000
			Capabilities.Touchpad = 0;			// 0x80000000
		}

		m_Thread = CreateThread(
			NULL,
			sizeof(size_t) * 4,
			UpdatePad,
			this,
			0,
			&m_ThreadId
		);

		if (m_Thread == NULL) {
			LOGW("Unable to create a thread for asynchronous updating.\n", hidDevice.Path);
		}
	}

	GamepadGeneric::~GamepadGeneric()
	{
		Terminate();
		FreeBuffer();
	}

	bool GamepadGeneric::IsAvailable() const
	{
		return !m_Closed;
	}

	void GamepadGeneric::Update()
	{
		if (m_Thread != NULL) {
			PressedButtons = m_BufButtons;
			m_BufButtons.Data = 0;

			LAxis = m_BufLAxis;
			RAxis = m_BufRAxis;
			Gyroscope = m_BufGyroscope;
			Accelerometer = m_BufAccelerometer;
			Touchpad = m_BufTouchpad;
		}
		else {
			InternalUpdate();
		}
	}

	void GamepadGeneric::InvalidateWriteBuffer() {
		m_IsInvalidated = true;
	}

	void GamepadGeneric::Terminate()
	{
		const DWORD TIMEOUT = 250;

		m_Closed = TRUE;
		WaitForSingleObject(m_Thread, TIMEOUT);

		CloseDevice();

		HANDLE thread = m_Thread;
		m_Thread = NULL;
		m_ThreadId = 0;
		TerminateThread(thread, 0);
	}

	bool GamepadGeneric::OpenDevice(const char * path, bool exclusive)
	{
		m_Handle = CreateFileA(path,
			GENERIC_READ | GENERIC_WRITE,
			exclusive ? FILE_SHARE_READ : FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, NULL);
		return m_Handle != INVALID_HANDLE_VALUE;
	}

	void GamepadGeneric::CloseDevice()
	{
		CloseHandle(m_Handle);
		m_Handle = NULL;
	}

	void GamepadGeneric::ReCreateBuffer(size_t sizeRead, size_t sizeWrite)
	{
		FreeBuffer();
		CreateBuffer(sizeRead, sizeWrite);
	}

	void GamepadGeneric::CreateBuffer(size_t sizeRead, size_t sizeWrite)
	{
		m_BufferRead = (u8*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_BufferReadLength = sizeRead);
		m_BufferWrite = (u8*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_BufferWriteLength = sizeWrite);
	}

	void GamepadGeneric::FreeBuffer()
	{
		if (m_BufferRead)
			HeapFree(GetProcessHeap(), 0, m_BufferRead);

		if (m_BufferRead)
			HeapFree(GetProcessHeap(), 0, m_BufferWrite);
	}

	bool GamepadGeneric::ReadBuffer()
	{
		if (m_BufferRead)
		{		
			DWORD read;
			if (ReadFile(m_Handle, m_BufferRead, m_BufferReadLength, &read, NULL)) {
				return true;
			}

			HandleError();
		}
		return false;
	}

	bool GamepadGeneric::WriteBuffer()
	{
		if (m_BufferWrite)
		{
			DWORD written;
			if (WriteFile(m_Handle, m_BufferWrite, m_BufferWriteLength, &written, NULL)) {
				return true;
			}

			HandleError();
		}
		return false;
	}

	void GamepadGeneric::HandleError() {
		DWORD error = GetLastError();
		switch (error) {
		case ERROR_INVALID_PARAMETER:
			LOGI("Gamepad invalid parameter.\n");
			break;
		case ERROR_OPERATION_ABORTED:
		case ERROR_DEVICE_NOT_CONNECTED:
			LOGI("Gamepad was disconnected.\n");
			Terminate();
			break;
		case ERROR_INVALID_USER_BUFFER:
			LOGI("Gamepad invalid user buffer.\n");
			Terminate();
			break;
		default:
			LOGW("Uncaught Gamepad error: %08X\n", error);
			Terminate();
			break;
		}
	}

	void GamepadGeneric::InternalUpdate()
	{
		while (IsAvailable()) {
			if (ReadBuffer() && IsAvailable()) {
				OnUpdate();
				if (IsAvailable() && m_IsInvalidated) {
					m_IsInvalidated = false;
					WriteBuffer();
				}
			}
		}
	}

	DWORD __stdcall GamepadGeneric::UpdatePad(LPVOID lpThreadParameter) {
		auto pad = (GamepadGeneric*)lpThreadParameter;
		if (pad) {
			pad->InternalUpdate();
		}

		return 0U;
	}
}