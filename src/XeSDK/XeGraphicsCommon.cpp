#include "pch.h"
#include "XeGraphicsCommon.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>

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
                m_Usage(usage)
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
            switch (m_Usage) {
                case Usage_Standard:
					break;
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
                LOGW("Resource already locked.");
                return false;
            }
            return m_isLocked = SubLock(map, type);
        }
        void IResource::Unlock()
        {
            if (!m_isLocked)
            {
                LOGW("Resource already unlocked.");
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

                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Color(&color)[4]) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[0];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[1];
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[2];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Color &color) {
                vOut->Position = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Color(&color)[4]) {
                vOut->Position = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], float z, const Color &color) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);

                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;

                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], float z, const Color(&color)[4]) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[0];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[1];
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[2];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.z = 1.0f;
                vOut->Color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], float z, const Color &color) {
                vOut->Position = Math::Vector3f(position[0], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[1], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[2], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[3], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], float z, const Color(&color)[4]) {
                vOut->Position = Math::Vector3f(position[0], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = Math::Vector3f(position[1], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = Math::Vector3f(position[2], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = Math::Vector3f(position[3], z).Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[3];
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Color &color) {
                vOut->Position = position[0].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[1].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[2].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[3].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color;
            }
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Color(&color)[4]) {
                vOut->Position = position[0].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = position[1].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = position[2].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = position[3].Transform(m);
                vOut->Texture.z = 1.0f;
                vOut->Color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color[0];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color[1];
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color[2];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = 0.0f;
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                vOut->Position = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                vOut->Position = Math::Vector3f(position[0], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = Math::Vector3f(position[1], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = Math::Vector3f(position[2], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = Math::Vector3f(position[3], 0.0f).Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
                const Vector2f v1 = position[0].Transform(m);
                const Vector2f v2 = position[1].Transform(m);
                vOut->Position.x = v1.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color[0];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v1.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color[1];
                vOut++;
                vOut->Position.x = v1.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color[2];
                vOut++;
                vOut->Position.x = v2.x;
                vOut->Position.y = v2.y;
                vOut->Position.z = z;
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
                vOut->Position = Math::Vector3f(position[0], z).Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[1], z).Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[2], z).Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = Math::Vector3f(position[3], z).Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
                vOut->Position = Math::Vector3f(position[0], z).Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = Math::Vector3f(position[1], z).Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = Math::Vector3f(position[2], z).Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = Math::Vector3f(position[3], z).Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color[3];
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
                vOut->Position = position[0].Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[1].Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[2].Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
                vOut++;
                vOut->Position = position[3].Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color;
            }
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
                vOut->Position = position[0].Transform(m);
                vOut->Texture.x = uvCoord[0].x;
                vOut->Texture.y = uvCoord[0].y;
                vOut->Texture.z = mode;
                vOut->Color = color[0];
                vOut++;
                vOut->Position = position[1].Transform(m);
                vOut->Texture.x = uvCoord[1].x;
                vOut->Texture.y = uvCoord[1].y;
                vOut->Texture.z = mode;
                vOut->Color = color[1];
                vOut++;
                vOut->Position = position[2].Transform(m);
                vOut->Texture.x = uvCoord[2].x;
                vOut->Texture.y = uvCoord[2].y;
                vOut->Texture.z = mode;
                vOut->Color = color[2];
                vOut++;
                vOut->Position = position[3].Transform(m);
                vOut->Texture.x = uvCoord[3].x;
                vOut->Texture.y = uvCoord[3].y;
                vOut->Texture.z = mode;
                vOut->Color = color[3];
            }
        }
    }
}