#pragma once

namespace RenderingDriverD3D11
{
	class CBuffer : public Xe::Graphics::IBuffer
	{
		ID3D11Buffer * m_pBuffer;
		void* m_LockBuffer;
		bool m_IsLocked;
		bool m_DirectMode;
	public:
		CBuffer(
			Xe::Graphics::IContext *pContext,
			const Xe::Graphics::BufferDesc& desc,
			ID3D11Buffer *pBuffer);
		~CBuffer();

		ID3D11Buffer* GetBuffer() const;

		bool SubLock(
			Xe::Graphics::DataDesc & map,
			Xe::Graphics::LockType type);
		void SubUnlock();
	};
}