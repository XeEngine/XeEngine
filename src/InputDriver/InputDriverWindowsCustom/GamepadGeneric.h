#pragma once
#include <XeSDK/XeIOGamepad.h>

namespace Xe {
	namespace IO {
		struct HidDevice;
	}

	class GamepadGeneric : public Xe::IO::Gamepad {
	public:
		GamepadGeneric(const Xe::IO::HidDevice& hidDevice);
		virtual ~GamepadGeneric();

		bool IsAvailable() const;
		void Update();

	protected:
		size_t m_BufferReadLength;
		size_t m_BufferWriteLength;
		u8* m_BufferRead;
		u8* m_BufferWrite;

		Buttons m_BufButtons;
		Xe::Math::Vector3f m_BufLAxis;
		Xe::Math::Vector3f m_BufRAxis;
		Xe::Math::Vector3f m_BufGyroscope;
		Xe::Math::Vector3f m_BufAccelerometer;
		Xe::Math::Vector2f m_BufTouchpad;

		static u8 ToU8(float data);
		static float FromS8(BYTE data);
		static float FromU8(BYTE data);

		virtual void OnUpdate() = 0;
		void InvalidateWriteBuffer();
		void Terminate();

		bool OpenDevice(const char* path, bool exclusive = false);
		void CloseDevice();

		void ReCreateBuffer(size_t sizeRead, size_t sizeWrite);
	private:
		HANDLE m_Handle;
		HANDLE m_Thread;
		DWORD m_ThreadId;
		bool m_Closed;
		bool m_IsInvalidated;

		void CreateBuffer(size_t sizeRead, size_t sizeWrite);
		void FreeBuffer();

		bool ReadBuffer();
		bool WriteBuffer();
		void HandleError();

		void InternalUpdate();

		static DWORD __stdcall UpdatePad(LPVOID lpThreadParameter);
	};
}