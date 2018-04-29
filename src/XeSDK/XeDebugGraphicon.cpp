#include "pch.h"
#include <XeSDK/XeDebugConsole.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsFont.h>
#include <XeSDK/XeGraphicsFontUV.h>

using namespace Xe;
using namespace Xe::Graphics;
using namespace Xe::Graphics::Font;

namespace Xe {
	namespace Debug {
		class Graphicon : public Xe::Debug::IConsole {
			Properties m_prop;
			Xe::Graphics::IContext *m_pContext;
			Xe::Graphics::Font::IFont *m_pFont;

			Xe::Graphics::Size m_fntSize;
			Xe::Graphics::Font::FontUV *m_pFontUv;
			Xe::Graphics::ISurface *m_pSrfFont;

			void DrawChar(tchar ch) {
				Font::FontUV::FontFrame chFrame;
				Position pos = m_prop.coord * m_fntSize;

				ISurface *surface;
				if (m_pFontUv->GetSurface(&surface)) {
					IDrawing2d *pDrawing;
					m_pContext->SelectSurface(surface, 0);
					m_pContext->GetDrawing(&pDrawing);

					bool chFound = m_pFontUv->GetCharacter(chFrame, ch);
					if (!chFound)
						chFound = m_pFontUv->GetCharacter(chFrame, m_pFontUv->GetDefaultCharacter());
					if (chFound) {
						Math::Vector2f posf[4];
						posf[0].x = (float)pos.x;
						posf[0].y = (float)pos.y;
						posf[1].x = (float)(pos.x + chFrame.size.x);
						posf[1].y = (float)posf[0].y;
						posf[2].y = (float)(pos.y + chFrame.size.y);
						posf[2].x = (float)posf[0].x;
						posf[3].x = (float)posf[1].x;
						posf[3].y = (float)posf[2].y;
						pDrawing->DrawSurface(posf, chFrame.uv, m_prop.foreColor, IDrawing2d::MODE_TEXTURED);
					}
					pDrawing->Release();
					surface->Release();
				}
			}
		public:
			bool Query(IObject **obj, Xe::UID id) {
				switch (id) {
				case IConsole::ID:
				case IObject::ID:
					this->AddRef();
					*obj = this;
					return true;
				}
				*obj = (IObject*)nullptr;
				return false;
			}

			Graphicon(Xe::Graphics::IContext *pContext, Xe::Graphics::Font::IFont *pFont) :
				m_pContext(pContext), m_pFont(pFont) {
				m_pContext->AddRef();
				m_pFont->AddRef();

				m_fntSize.x = m_pFont->GetPxWidth();
				m_fntSize.y = m_pFont->GetPxHeight();
				m_pFontUv = new FontUV(m_pFont, m_pContext, Color::Format_BGRA8888);
				m_pFontUv->GetSurface(&m_pSrfFont);
			}
			~Graphicon() {
				m_pSrfFont->Release();
				m_pFontUv->Release();
				m_pContext->Release();
				m_pFont->Release();
			}

			const Properties &GetProperties() const {
				return m_prop;
			}
			void SetProperties(const Properties& properties) {
				m_prop = properties;
			}
			const Color &GetForeColor() const {
				return m_prop.foreColor;
			}
			void SetForeColor(const Color &color) {
				m_prop.foreColor = color;
			}
			void Clear() {
				m_pContext->SetClearColor(m_prop.backColor);
				m_pContext->Clear(Clear_Color);
			}
			void PutChar(tchar ch) {
				switch (ch) {
				case '\b':
					if (m_prop.coord.x > 0) {
						m_prop.coord.x--;
						DrawChar(' ');
					}
					break;
				case '\n':
					m_prop.coord.x = 0;
					m_prop.coord.y += 1;
					break;
				case '\r':
					m_prop.coord.x = 0;
					m_prop.coord.y += 1;
					break;
				case '\t':
					m_prop.coord.x = (m_prop.coord.x + 4) & ~3;
					break;
				case ' ':
					m_prop.coord.x += 1;
					break;
				default:
					DrawChar(ch);
					m_prop.coord.x += 1;
					break;
				}
			}
			void Print(ctstring fmt, ...) {
				tchar buf[0x100];
				void *args;
				GET_ARGS_ADDRESS(args, fmt);
				int n = Xe::String::PrintArguments(buf, sizeof(buf), fmt, args);
				for (int i = 0; i < n; i++)
					PutChar(buf[i]);
			}
		};
		bool CreateGraphicon(IConsole **console, Graphics::IContext *pContext, Xe::Graphics::Font::IFont *pFont) {
			*console = new Graphicon(pContext, pFont);
			return true;
		}
	}
}