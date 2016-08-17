/*#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeGameComponentTexture2d.h>

namespace Xe {
	namespace Game {
		namespace Component {
			class Sprite : public Texture2d {
			public:
				static const UID ID = 0x3933a7f8d3804fdaULL;
				bool Query(IObject **obj, UID id);

				Sprite();
				~Sprite();

				virtual void OnUpdate(float update);
				virtual void OnDraw();

				bool GetSurface(Graphics::ISurface **surface);
				void SetSurface(Graphics::ISurface *surface);

				void SetSquareCoord(float left, float top, float right, float bottom);
				void SetUVCoord(const Math::Vector2f(&coord)[4]);
				const Math::Vector2f(&GetUVCoord() const)[4];

			private:
				Graphics::ISurface *m_pSurface;
				Math::Vector2f m_uv[4];
			};
		}
	}
}*/