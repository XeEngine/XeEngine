#include "pch.h"
#include "XeGraphicsCommon.h"
#include <XeSDK/IGraphicsContext.h>

using namespace Xe::Debug;

namespace Xe {
    namespace Graphics {
        Capabilities::Capabilities() :
                MaxTextureUnits(0),
                MaxTextureSize(0) { }
        DataDesc::DataDesc() :
                data(nullptr), pitch(0) {}
        DataDesc::DataDesc(const void *data, svar pitch) :
                data(data), pitch(pitch) {}

        IResource::IResource(IContext* pContext, UsageType usage) :
                m_pContext(pContext),
                m_usage(usage)
        {
            m_pContext->AddRef();
            m_length = 0;
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
            return m_length;
        }
        UsageType IResource::GetUsage() const
        {
            return m_usage;
        }
        bool IResource::IsLocked() const
        {
            return m_isLocked;
        }
        bool IResource::Lock(DataDesc& map, LockType type)
        {
            switch (m_usage) {
                case Usage_Standard:
                    return false;
                case Usage_Static:
                    return false;
                case Usage_Dynamic:
                    if (type != Lock_Read)
                        return false;
                    break;
                case Usage_Full:
                    break;
            }

            if (m_isLocked)
            {
                LOG(Log::Priority_Warning, Log::Type_Graphics, _T("Resource already locked."));
                return false;
            }
            return m_isLocked = SubLock(map, type);
        }
        void IResource::Unlock()
        {
            if (!m_isLocked)
            {
                LOG(Log::Priority_Warning, Log::Type_Graphics, _T("Resource already unlocked."));
                return;
            }
            SubUnlock();
            m_isLocked = false;
        }

        IBuffer::IBuffer(IContext* pContext, UsageType usage, svar length,
                         BufferType type) : IResource(pContext, usage),
                                            m_type(type)
        {
            m_length = length;
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

        namespace Common {
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Color &color) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);

                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Color(&color)[4]) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color[0];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color[1];
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color[2];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->p = 1.0f;
                vOut->color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Color &color) {
                vOut->pos = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Color(&color)[4]) {
                vOut->pos = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[0];
                vOut++;
                vOut->pos = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[1];
                vOut++;
                vOut->pos = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[2];
                vOut++;
                vOut->pos = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], float z, const Color &color) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);

                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;

                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], float z, const Color(&color)[4]) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color[0];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color[1];
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color[2];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->p = 1.0f;
                vOut->color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], float z, const Color &color) {
                vOut->pos = Math::Vector3f(position[0], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[1], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[2], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[3], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], float z, const Color(&color)[4]) {
                vOut->pos = Math::Vector3f(position[0], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[0];
                vOut++;
                vOut->pos = Math::Vector3f(position[1], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[1];
                vOut++;
                vOut->pos = Math::Vector3f(position[2], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[2];
                vOut++;
                vOut->pos = Math::Vector3f(position[3], z).Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Color &color) {
                vOut->pos = position[0].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = position[1].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = position[2].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
                vOut++;
                vOut->pos = position[3].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Color(&color)[4]) {
                vOut->pos = position[0].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[0];
                vOut++;
                vOut->pos = position[1].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[1];
                vOut++;
                vOut->pos = position[2].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[2];
                vOut++;
                vOut->pos = position[3].Transform(m);
                vOut->p = 1.0f;
                vOut->color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color[0];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color[1];
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color[2];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = 0.0f;
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                vOut->pos = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                vOut->pos = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color[0];
                vOut++;
                vOut->pos = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color[1];
                vOut++;
                vOut->pos = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color[2];
                vOut++;
                vOut->pos = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->pos.x = v1.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color[0];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v1.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color[1];
                vOut++;
                vOut->pos.x = v1.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color[2];
                vOut++;
                vOut->pos.x = v2.x;
                vOut->pos.y = v2.y;
                vOut->pos.z = z;
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
                vOut->pos = Math::Vector3f(position[0], z).Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[1], z).Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[2], z).Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = Math::Vector3f(position[3], z).Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
                vOut->pos = Math::Vector3f(position[0], z).Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color[0];
                vOut++;
                vOut->pos = Math::Vector3f(position[1], z).Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color[1];
                vOut++;
                vOut->pos = Math::Vector3f(position[2], z).Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color[2];
                vOut++;
                vOut->pos = Math::Vector3f(position[3], z).Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                vOut->pos = position[0].Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = position[1].Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = position[2].Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color;
                vOut++;
                vOut->pos = position[3].Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                vOut->pos = position[0].Transform(m);
                vOut->u = uvCoord[0].x;
                vOut->v = uvCoord[0].y;
                vOut->p = mode;
                vOut->color = color[0];
                vOut++;
                vOut->pos = position[1].Transform(m);
                vOut->u = uvCoord[1].x;
                vOut->v = uvCoord[1].y;
                vOut->p = mode;
                vOut->color = color[1];
                vOut++;
                vOut->pos = position[2].Transform(m);
                vOut->u = uvCoord[2].x;
                vOut->v = uvCoord[2].y;
                vOut->p = mode;
                vOut->color = color[2];
                vOut++;
                vOut->pos = position[3].Transform(m);
                vOut->u = uvCoord[3].x;
                vOut->v = uvCoord[3].y;
                vOut->p = mode;
                vOut->color = color[3];
            }
        }
    }
}