#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/IGameComponent.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathVector2.h>

namespace Xe {
	namespace Game {
		namespace Component {
			class Texture2d : public IComponent {
			public:
				Texture2d();
				~Texture2d();

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
}