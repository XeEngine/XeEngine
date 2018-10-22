#pragma once

namespace RenderingDriverNull
{
	class CBuffer : public Xe::Graphics::IBuffer
	{
		typedef IBuffer Base;

		void* m_Data;
		void* m_BackData;
		bool m_CopyBack;
	public:
		CBuffer(
			Xe::Graphics::IContext* pContext,
			Xe::Graphics::UsageType usage,
			svar length,
			Xe::Graphics::BufferType type);
		virtual ~CBuffer();

		bool SubLock(
			Xe::Graphics::DataDesc& map,
			Xe::Graphics::LockType type);
		void SubUnlock();
	};
}