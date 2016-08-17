#include "pch.h"
#include <XeSDK/XeGameTiled.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeString.h>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace Xe::Debug;

namespace Xe {
	namespace Game {
		class CTiled : public Tiled {
			enum Encoding {
				//Encoding_Cvs,
				Encoding_Base64,
			};
			struct Layer {
				const uvar Width, Height;
				Graphics::Tilemap2d::Tile *Map;
				Math::Vector2f m_Position;
				bool m_IsPositionInvalidated;
				Math::Vector2f m_Margin[2];

				Layer(uvar width, svar height) :
					Width(width), Height(height),
					Map((Graphics::Tilemap2d::Tile*)Memory::Alloc(sizeof(u32) * width * height))
				{}
				~Layer() {
					Memory::Free(Map);
				}
			};

			Graphics::Tilemap2d *m_pTilemap;
			Math::Vector2f m_InternalTileSize;
			Math::Vector2f m_InternalTilemapSize;
			Math::Vector2f m_position;
			tinyxml2::XMLDocument m_xmldoc;
			char *m_xmldata;

			int m_MapWidth, m_MapHeight;
			int m_TileWidth, m_TileHeight;
			Layer *m_pLayer;
			Math::Vector2f m_TilemapSize;
			Math::Vector2f m_TilemapOrigin[2];

			bool m_IsInvalidated;
			void ValidateLayerPosition(Layer& layer);
			bool Base64_Decode(u8 *dst, const char *src, uvar dstlen);
			void DecodeMap(Layer *layer, Encoding encoding, const char *data);
		public:
			bool Query(IObject **ppObject, UID id);

			CTiled(Graphics::Tilemap2d *pTilemap);
			~CTiled();
			bool Open(IO::IStream *pStream);
			void Update(double deltaTime);
			void SetPosition(const Math::Vector2f& position);
		};

		bool CTiled::Query(IObject **ppObject, UID id) {
			switch (id) {
			case Tiled::ID:
			case IObject::ID:
				AddRef();
				*ppObject = this;
				return true;
			}
			*ppObject = nullptr;
			return false;
		}
		CTiled::CTiled(Graphics::Tilemap2d *pTilemap) :
			m_pTilemap(pTilemap),
			m_xmldata(nullptr),
			m_pLayer(nullptr),
			m_IsInvalidated(true) {
			m_pTilemap->AddRef();

			m_InternalTileSize.x = m_pTilemap->TileSize;
			m_InternalTileSize.y = m_pTilemap->TileSize;
			m_InternalTilemapSize.x = m_pTilemap->TilemapWidth;
			m_InternalTilemapSize.y = m_pTilemap->TilemapHeight;
		}
		CTiled::~CTiled() {
			if (m_pLayer) delete m_pLayer;
			if (m_xmldata) Memory::Free(m_xmldata);
			m_pTilemap->Release();
		}
		bool CTiled::Open(IO::IStream *pStream) {
			s64 len = pStream->GetLength();
			m_xmldata = (char*)Memory::Alloc((uvar)len);
			pStream->Read(m_xmldata, 0, (s32)len);
			if (m_xmldoc.Parse(m_xmldata, (size_t)len) == 0) {
				XMLElement *pElement = m_xmldoc.FirstChildElement("map");
				if (pElement) {
					bool r = pElement->Attribute("orientation", "orthogonal") != nullptr;
					if (r) r &= pElement->Attribute("renderorder", "right-down") != nullptr;
					if (r) r &= pElement->QueryIntAttribute("width", &m_MapWidth) == 0;
					if (r) r &= pElement->QueryIntAttribute("height", &m_MapHeight) == 0;
					if (r) r &= pElement->QueryIntAttribute("tilewidth", &m_TileWidth) == 0;
					if (r) r &= pElement->QueryIntAttribute("tileheight", &m_TileHeight) == 0;
					if (r) {
						XMLElement *pElementLayer = pElement->FirstChildElement("layer");
						if (pElementLayer) {
							int width, height;
							pElementLayer->QueryIntAttribute("width", &width);
							pElementLayer->QueryIntAttribute("height", &height);
							m_pLayer = new Layer(width, height);
							XMLElement *pElementData = pElementLayer->FirstChildElement("data");
							if (pElementLayer) {
								const char *strEncoding = pElementData->Attribute("encoding");
								if (String(strEncoding).Compare(_T("base64"), -1, 0, String::Compare_IgnoreCase) == 0) {
									DecodeMap(m_pLayer, Encoding_Base64, pElementData->GetText());
								}
								else LOG(Log::Priority_Error, Log::Type_Module, _T("Encoding different from base64 are not supported."));
							}
							else LOG(Log::Priority_Error, Log::Type_Module, _T("Unable to find <data>."));
							return true;
						}
						else LOG(Log::Priority_Error, Log::Type_Module, _T("Unable to find <layer>."));
					}
					else LOG(Log::Priority_Error, Log::Type_Module, _T("Invalid map."));
				}
				else LOG(Log::Priority_Error, Log::Type_Module, _T("Unable to find <map>."));
			}
			else LOG(Log::Priority_Error, Log::Type_Module, _T("tinyxml2 err %i during CTiled parsing."), m_xmldoc.ErrorID());

			Memory::Free(m_xmldata);
			m_xmldata = nullptr;
			return false;
		}
		void CTiled::Update(double deltaTime) {
			if (m_IsInvalidated) {
				m_IsInvalidated = false;
				svar w = Math::Min<svar>(m_pTilemap->TilemapWidth, m_pLayer->Width);
				svar h = Math::Min<svar>(m_pTilemap->TilemapHeight, m_pLayer->Height);
				for (svar i = 0; i < h; i++) {
					Memory::Copy(m_pTilemap->Tilemap + i * m_pTilemap->TilemapWidth,
						m_pLayer->Map + i * m_pLayer->Width,
						sizeof(u32) * w);
				}
				m_pTilemap->Invalidate();
				m_pLayer[0].m_Margin[1] = m_InternalTilemapSize;
			}
			for (svar i = 0; i < 1; i++) {
				Layer *pLayer = m_pLayer + i;
				if (pLayer->m_IsPositionInvalidated)
					ValidateLayerPosition(*pLayer);
			}
		}
		void CTiled::SetPosition(const Math::Vector2f& position) {
			/*Math::Vector2f pos = position.Max(Math::Vector2f(0.0f, 0.0f));
			//pos = position;
			const svar layer = 0;

			const Math::Vector2f& viewSize = m_pTilemap->GetViewSize();
			Math::Vector2f diff = (viewSize + pos) / m_InternalTileSize - m_TilemapOrigin.x;
			if (diff.x > m_InternalTilemapSize.x) {
				svar prev = (svar)m_TilemapOrigin.x;
				m_TilemapOrigin.x += Math::Ceiling(diff.x - m_InternalTilemapSize.x);
				svar x = (svar)m_TilemapOrigin.x;
				svar columns = prev + x;

				for (svar j = 0; j < columns; j++) {
					Graphics::Tilemap2d::Tile *pTile = m_pTilemap->Tilemap + (j) % m_pTilemap->TilemapWidth;
					for (svar i = 0; i < m_pTilemap->TilemapHeight; i++) {
						pTile[i * m_pTilemap->TilemapWidth] =
							m_pLayer->Map[x + i + i * m_pLayer->Width];
					}
				}
				m_pTilemap->Invalidate();
			}
			else if (diff.x < 0) {
				//m_TilemapOrigin.x -= Math::Ceiling(diff.x - m_InternalTilemapSize.x);
			}
			diff = (viewSize + pos) / m_InternalTileSize - m_TilemapOrigin;
			LOG(Log::Priority_Diagnostics, Log::Type_Module, _T("diff(%f, %f)"), diff.x, diff.y);*/

			const svar layer = 0;
			Math::Vector2f pos = position.Max(Math::Vector2f(0.0f, 0.0f));
			m_pLayer[layer].m_IsPositionInvalidated = true;
			m_pLayer[layer].m_Position = pos;
			m_pTilemap->SetPosition(layer, pos);
		}

		void CTiled::ValidateLayerPosition(Layer& layer) {
			layer.m_IsPositionInvalidated = false;

			const Math::Vector2f& viewSize = m_pTilemap->GetViewSize();
			const Math::Vector2f viewSizePlus = viewSize + m_InternalTileSize;
			Math::Vector2f m1 = layer.m_Position / m_InternalTileSize; // Top-left margin
			Math::Vector2f m2 = (viewSizePlus + layer.m_Position) / m_InternalTileSize; // Right-bottom margin
			
			m2.x = Math::Ceiling(m2.x);
			if (m2.x > layer.m_Margin[1].x) {
				svar dstx = (svar)layer.m_Margin[1].x;
				svar srcx = (svar)m2.x;
				svar cols = srcx - (svar)layer.m_Margin[1].x;
				for (svar i = 0; i < cols; i++, dstx++) {
					Graphics::Tilemap2d::Tile *pTile = m_pTilemap->Tilemap + (dstx % m_pTilemap->TilemapWidth);
					for (svar j = 0; j < m_pTilemap->TilemapHeight; j++) {
						pTile[j * m_pTilemap->TilemapWidth] =
							m_pLayer->Map[i + dstx + j + j * m_pLayer->Width];
					}
				}

				layer.m_Margin[0].x = m2.x - m_InternalTilemapSize.x;
				layer.m_Margin[1].x = m2.x;
			}
			m1 = layer.m_Margin[0];
			m2 = layer.m_Margin[1];
			LOG(Log::Priority_Diagnostics, Log::Type_Module, _T("m1(%f, %f)  m2(%f, %f)"), m1.x, m1.y, m2.x, m2.y);
		}

		// TMX SPECIFICS
		bool CTiled::Base64_Decode(u8 *dst, const char *src, uvar dstlen) {
			static const u8 TABLE[0x100] =
			{
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
				0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xCC, 0xFF, 0xFF,
				0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
				0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
				0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			};
			const u8 *pSrc = (const u8*)src;
			u8 *pDst = dst;
			u8 *pDstMax = dst + dstlen;
			unsigned long t, y;
			unsigned char c;
			svar g;

			g = 3;
			for (y = t = 0; *pSrc;) {
				c = TABLE[*pSrc++ & 0xFF];
				if (c == 0xFF) continue;
				/* the final = symbols are read and used to trim the remaining bytes */
				if (c == 0xCC) {
					c = 0;
					/* prevent g < 0 which would potentially allow an overflow later */
					if (--g < 0) {
						return false;
					}
				}
				else if (g != 3) {
					/* we only allow = to be at the end */
					return false;
				}

				t = (t << 6) | c;

				if (++y == 4) {
					if (pDst + g > pDstMax)
						return false;
					*pDst++ = (u8)(t >> 16);
					if (g > 1) *pDst++ = (u8)(t >> 8);
					if (g > 2) *pDst++ = (u8)t;
					y = t = 0;
				}
			}
			if (y != 0) return false;
			return true;
		}
		void CTiled::DecodeMap(Layer *layer, Encoding encoding, const char *data) {
			uvar len = sizeof(u32) * layer->Width * layer->Height;
			Base64_Decode((u8*)layer->Map, data, len);
			for (uvar i = 0; i < layer->Width * layer->Height; i++) {
				if (layer->Map[i].tile > 0)
					layer->Map[i].tile--;
				layer->Map[i].data = layer->Map[i].tile | (layer->Map[i].data & 0xF0000000);
			}
		}

		bool Tiled::Open(Tiled **ppTiled, Graphics::Tilemap2d *pTilemap, IO::IStream *pStream) {
			CTiled *t = new CTiled(pTilemap);
			if (t->Open(pStream)) {
				*ppTiled = t;
				return true;
			}
			t->Release();
			*ppTiled = nullptr;
			return false;
		}
	}
}