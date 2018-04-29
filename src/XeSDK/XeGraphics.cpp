#include "pch.h"
#include <XeSDK/XeGraphics.h>
//#include "XeGraphics_RenderD3D11.h"
//#include "XeGraphics_GLES.h"

using namespace Xe::Debug;

namespace Xe {
    namespace Graphics {
        bool CreateContextNull(IContext **context, Core::IView* pView, const ContextProperties& properties);
        bool CreateContextGL(IContext **context, Core::IView* pView, const ContextProperties& properties);
        bool CreateContextD3D11(IContext **context, Core::IView* pView, const ContextProperties& properties);

        RESULT Create(IContext **context, Core::IView* pView, const ContextProperties& properties) {
            static const ctstring RenderEngineStr[] = {
                    _T("Instance"),
                    _T("NULL"),
                    _T("D3D9"),
                    _T("D3D11"),
                    _T("D3D12"),
                    _T("GL21"),
                    _T("GL33"),
                    _T("GLES20"),
            };

            *context = 0;
            RenderEngine renderer = properties.Render;
            ctstring renderEngineName = RenderEngineStr[renderer];
            if (renderer != RenderEngine_Default)
            {
                LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Checking if render engine %s is supported..."), renderEngineName);
                if (renderer != RenderEngine_Null) {
                    if (!IsSupported(renderer)) {
                        LOG(Log::Priority_Error, Log::Type_Graphics, _T("Render engine %s is not supported."), renderEngineName);
                        return Error::GENERIC_ERROR;
                    }
                }
            }
            else {
                LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Default render engine specified."));
                renderer = GetDefaultRenderEngine();
            }
            renderEngineName = RenderEngineStr[renderer];
            LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Creating render engine %s..."), renderEngineName);
            bool (*pfnCreateContext)(IContext **pContext, Core::IView* pView, const ContextProperties& properties);
            switch (renderer) {
                case RenderEngine_Null:
                    pfnCreateContext = CreateContextNull;
                    break;
                case RenderEngine_D3D9:
                    pfnCreateContext = 0;
                    break;
                case RenderEngine_D3D11:
                    pfnCreateContext = CreateContextD3D11;
                    break;
                case RenderEngine_D3D12:
                    pfnCreateContext = 0;
                    break;
                case RenderEngine_OpenGL:
#if defined(PLATFORM_GL)
                    pfnCreateContext = CreateContextGL;
#else
                    pfnCreateContext = 0;
#endif
                    break;
                    /*case RenderEngine_GL33:
                        pfnCreateContext = CreateContextGL;
                        break;
                    case RenderEngine_GLES20:
                        pfnCreateContext = CreateContextGLES;
                        break;*/
                default:
                    LOG(Log::Priority_Error, Log::Type_Graphics, _T("Invalid render engine specified (%i)."), renderEngineName);
                    return Error::INVALID_PARAMETER;
            }
            if (pfnCreateContext != 0) {
                if (pfnCreateContext(context, pView, properties)) {
                    LOG(Log::Priority_Info, Log::Type_Graphics, _T("Render engine %s was created."), renderEngineName);
                    return Error::OK;
                }
                LOG(Log::Priority_Info, Log::Type_Graphics, _T("Unable to create render engine %s."), renderEngineName);
                return Error::GENERIC_ERROR;
            }
            LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Ooops, render engine %s is not yet supported"), renderEngineName);
            return Error::NOT_IMPLEMENTED;
        }
    }
}