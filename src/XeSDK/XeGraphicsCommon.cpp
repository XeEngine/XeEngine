#include "pch.h"
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
    namespace Graphics {
        DataDesc::DataDesc() :
                data(nullptr), pitch(0) {}
        DataDesc::DataDesc(const void *data, svar pitch) :
                data(data), pitch(pitch) {}

        IResource::IResource(IContext* pContext, UsageType usage) :
                m_pContext(pContext),
                m_Usage(usage)
        {
            m_pContext->AddRef();
			m_Length = 0;
            m_isLocked = false;
        }
        IResource::~IResource()
        {
            if (IsLocked())
                Unlock();
            m_pContext->Release();
        }
        void IResource::GetContext(IContext** ppContext) const
        {
            m_pContext->AddRef();
            *ppContext = m_pContext;
        }
        svar IResource::GetLength() const
        {
            return m_Length;
        }
        UsageType IResource::GetUsageType() const
        {
            return m_Usage;
        }
        bool IResource::IsLocked() const
        {
            return m_isLocked;
        }
        bool IResource::Lock(DataDesc& map, LockType type)
        {
			map.data = nullptr;
			map.pitch = 0;

			if (m_Usage == Usage_Static)
			{
				LOGE("Unable to lock a static resource");
				return false;
			}

			if (m_isLocked)
			{
				LOGE("Resource has been previously locked.");
				return false;
			}

            switch (m_Usage)
			{
                case Usage_Standard:
					break;
                case Usage_Static:
                    return false;
                case Usage_Dynamic:
                    break;
                case Usage_Full:
                    break;
            }

            return m_isLocked = SubLock(map, type);
        }
        void IResource::Unlock()
        {
            if (!m_isLocked)
            {
				LOGE("Resource has not been previously locked.");
                return;
            }

            SubUnlock();
            m_isLocked = false;
        }

        IBuffer::IBuffer(IContext* pContext, UsageType usage, svar length,
                         BufferType type) : IResource(pContext, usage),
                                            m_type(type)
        {
			m_Length = length;
        }
        IBuffer::~IBuffer()
        {}
        BufferType IBuffer::GetType() const
        {
            return m_type;
        }

        DepthStencilStateDesc DepthStencilStateDesc::Default() {
            DepthStencilStateDesc desc;
            desc.depthTestEnabled = true;
            desc.depthWriteEnabled = true;
            desc.depthComparison = Comparison_LessEqual;
            desc.stencilTestEnabled = false;
            desc.stencilMask = 0xFF;
            desc.stencilFrontFaceComp = Comparison_Always;
            desc.stencilBackFaceComp = Comparison_Always;
            return desc;
        }
    }
}