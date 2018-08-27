#include "pch.h"
#include <XeSDK/XeCoreFrameView.h>
#include "UwpFrameView.h"
#include "WinRtUtilities.h"

namespace Xe { namespace Core {

	struct CFrameworkViewSource :
		public ABI::Windows::ApplicationModel::Core::IFrameworkViewSource
	{
		int m_ref;
		Xe::Core::IFrameHandler* m_FrameHandler;
		UwpFrameView* m_FrameView;

		CFrameworkViewSource(Xe::Core::IFrameHandler* pFrameHandler)
		{
			m_FrameHandler = pFrameHandler;
		}

		// Inherted from IFrameworkViewSource
		HRESULT WINAPI CreateView(IFrameworkView** result)
		{
			m_FrameView = new UwpFrameView(m_FrameHandler);
			m_FrameHandler->OnAttach(m_FrameView);
			*result = m_FrameView;

			return S_OK;
		}

#pragma region IUnknown

		HRESULT WINAPI QueryInterface(IID const & id, void ** result)
		{
			if (result == nullptr) return E_POINTER;
			if (id == __uuidof(IFrameworkViewSource) ||
				id == __uuidof(IInspectable) ||
				id == __uuidof(IUnknown))
			{
				*result = static_cast<IFrameworkViewSource *>(this);
			}
			else if (id == __uuidof(IFrameworkView))
			{
				*result = this;
			}
			else
			{
				*result = nullptr;
				return E_NOINTERFACE;
			}
			static_cast<IUnknown *>(*result)->AddRef();
			return S_OK;
		}

		ULONG WINAPI AddRef()
		{
			return ++m_ref;
		}

		ULONG WINAPI Release()
		{
			if (--m_ref) return m_ref;
			delete this;
			return 0;
		}


#pragma endregion

#pragma region IInspectable

		HRESULT WINAPI GetIids(ULONG *, IID **)
		{
			return E_NOTIMPL;
		}

		HRESULT WINAPI GetRuntimeClassName(HSTRING *)
		{
			return E_NOTIMPL;
		}

		HRESULT WINAPI GetTrustLevel(TrustLevel *)
		{
			return E_NOTIMPL;
		}

#pragma endregion
	};

	bool Run(Xe::Core::IFrameHandler* pFrameHandler, const FrameViewInitDesc& desc)
	{
		ASSERT(pFrameHandler);

		HR(RoInitialize(RO_INIT_MULTITHREADED));
		auto CoreApplication = GetActivationFactory<ICoreApplication>(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);

		auto frameworkViewSource = new CFrameworkViewSource(pFrameHandler);
		bool result = CoreApplication->Run(frameworkViewSource) == S_OK;
		frameworkViewSource->Release();

		return result;
	}
} }