#pragma once

namespace RenderingDriverD3D11
{
	class CSurface : public Xe::Graphics::ISurface
	{
		void* m_LockBuffer;
		bool m_DirectMode;
	public:
		ID3D11Resource *m_pResource;
		ID3D11ShaderResourceView *m_pResourceView;
		ID3D11RenderTargetView *m_pTargetView;

		CSurface(
			Xe::Graphics::IContext *context,
			Xe::Graphics::SurfaceType type,
			const Xe::Graphics::Size &size,
			Xe::Graphics::Color::Format format,
			ID3D11Resource *resource,
			ID3D11ShaderResourceView *resourceView,
			ID3D11RenderTargetView *targetView);
		~CSurface();

		// Inherited via ISurface
		bool SubLock(
			Xe::Graphics::DataDesc & map,
			Xe::Graphics::LockType type);
		void SubUnlock();
	};
}