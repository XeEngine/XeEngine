#pragma once

namespace RenderingDriverNull
{
	class CSurface : public Xe::Graphics::ISurface
	{
		void* m_Data;
		void* m_BackData;
		bool m_CopyBack;
	public:
		CSurface(
			Xe::Graphics::IContext *context,
			Xe::Graphics::SurfaceType type,
			const Xe::Graphics::Size &size,
			Xe::Graphics::Color::Format format);
		~CSurface();

		// Inherited via ISurface
		bool SubLock(
			Xe::Graphics::DataDesc & map,
			Xe::Graphics::LockType type);
		void SubUnlock();
	};
}