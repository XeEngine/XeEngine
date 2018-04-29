#include "pch.h"
#include <XeSDK/XeGraphicsFontUV.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
    namespace Graphics {
        namespace Font {
            bool FontUV::Query(IObject **obj, UID id) {
                switch (id) {
                    case IFont::ID:
                        *obj = m_font;
                        m_font->AddRef();
                        return true;
                    case FontUV::ID:
                    case IObject::ID:
                        *obj = this;
                        AddRef();
                        return true;
                }
                *obj = nullptr;
                return false;
            }
            FontUV::FontUV(IFont *font, Graphics::IContext *context, Graphics::Color::Format format) :
                    m_font(font),
                    m_context(context),
                    m_surface(nullptr),
                    m_pUv(nullptr) {
                m_font->AddRef();
                m_context->AddRef();
                LOG(Debug::Log::Priority_Info, Debug::Log::Type_Font,
                    _T("Creating a FontUV from font %p (%s)..."), font, (ctstring)font->GetName());
                m_chFirst = m_font->GetChFirst();
                m_chCount = m_font->GetChCount();
                m_chDefault = m_font->GetDefaultCharacter();
                if (CreateSurface(format))
                    CreateUV();
                else {
                    LOG(Debug::Log::Priority_Warning, Debug::Log::Type_Font,
                        _T("There was an error creating surface from font %p (%s)"), font, (ctstring)font->GetName());
                }
                LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                    _T("FontUV created with success!"));
            }
            FontUV::~FontUV() {
                if (m_pUv)
                    delete[] m_pUv;
                m_font->Release();
                m_context->Release();
            }

            bool FontUV::GetSurface(Graphics::ISurface **surface) {
                *surface = m_surface;
                m_surface->AddRef();
                return m_surface != nullptr;
            }
            bool FontUV::GetCharacter(FontFrame &frame, uint ch) {
                ch -= m_chFirst;
                if (ch < (uint)m_chCount) {
                    Math::Vector2f *uv = &m_pUv[ch * 4];
                    Memory::Copy(frame.uv, uv, sizeof(frame.uv));
                    frame.size.x = m_font->GetPxWidth();
                    frame.size.y = m_font->GetPxHeight();
                    return true;
                }
                return false;
            }
            uint FontUV::GetDefaultCharacter() const {
                return m_chDefault;
            }

            svar FontUV::GetNearestPowerOfTwo(svar x) {
                static const short NEAREST_POWER2[] = {
                        16, 32, 64, 128, 256, 512, 1024, 2048
                };
                for (svar i = 0; i < lengthof(NEAREST_POWER2); i++)
                    if (x <= NEAREST_POWER2[i])
                        return NEAREST_POWER2[i];
                return -1;
            }
            const Size FontUV::GetSurfaceSize(const Size &size, svar count, bool powerOfTwo /*IGNORED*/) {
                static const short ALIGNED_PACKING[] = {
                        0, 16, 32, 64, 64, 256, 128, 64,
                        128, 128, 512, 1024, 256, 256, 256, 256,
                        256, 256, 128, 64, 1024, 64, 2048, 2048,
                        512, 128, 1024, 256, 256, 1024, 512, 1024,
                        512, 1024, 512, 1024, 256, 2048, 512, 512,
                };
                if (size.x < lengthof(ALIGNED_PACKING)) {
                    svar srfw = ALIGNED_PACKING[size.x];
                    svar perRow = srfw / size.x;
                    svar srfh = GetNearestPowerOfTwo(((count + perRow - 1) / perRow) * size.y);
                    return Size(srfw, srfh);
                }
                return Size(-1, -1);

            }
            bool FontUV::CreateSurface(Color::Format format) {
                LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                    _T("Creating font surface..."));

                format = Color::Format_BGRX8888;
                svar w = m_font->GetPxWidth();
                svar h = m_font->GetPxHeight();
                const Size &size = GetSurfaceSize(Size(w, h), m_chCount, true);
                DataDesc desc;
                desc.pitch = size.x * Color::GetBitsPerPixel(format) / 8;
                uint *data = (uint*)Memory::Alloc(desc.pitch * size.y);
                desc.data = data;

                svar chPerRow = size.x / w;
                for (svar i = 0; i < m_chCount; i++) {
                    svar x = i % chPerRow;
                    svar y = i / chPerRow;
                    uint *pDst = data + w * x + desc.pitch * h * y / 4;
                    m_font->ReadCharater(pDst, desc.pitch, h, format, m_chFirst + i);
                }
                return m_context->CreateSurface(&m_surface, SurfaceType_Texture, size, Color::Format_BGRA8888, desc);
            }
            void FontUV::CreateUV() {
                LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                    _T("Defining UV..."));
                svar w = m_font->GetPxWidth();
                svar h = m_font->GetPxHeight();
                const Size &size = m_surface->GetSize();
                svar chPerRow = size.x / w;

                m_pUv = new Math::Vector2f[m_chCount * 4];
                Math::Vector2f *pUv = m_pUv;
                Math::Vector2f sizef = Math::Vector2f((float)size.x, (float)size.y);
                for (svar ch = 0, y = 0; ch < m_chCount; y++) {
                    float v1 = (float)(y * h) / sizef.y;
                    float v2 = (float)(y * h + h) / sizef.y;
                    for (svar x = 0; x < chPerRow && ch < m_chCount; x++, ch++) {
                        pUv[0].x = pUv[2].x = (float)(x * w) / sizef.x;
                        pUv[0].y = pUv[1].y = v1;
                        pUv[1].x = pUv[3].x = (float)(x * w + w) / sizef.x;
                        pUv[2].y = pUv[3].y = v2;
                        pUv += 4;
                    }
                }
            }
        }
    }
}