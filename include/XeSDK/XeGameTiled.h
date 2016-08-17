#pragma once
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeGraphicsTilemap2d.h>
#include <XeSDK/IIOStream.h>

namespace Xe {
	namespace Game {
		class Tiled : public IObject {
		public:
			static const UID ID = 0x7fc8778865594913ULL;

			virtual void Update(double deltaTime) = 0;
			virtual void SetPosition(const Math::Vector2f& position) = 0;

			static bool Open(Tiled **ppTiled, Graphics::Tilemap2d *pTilemap, IO::IStream *pStream);
		};
	}
}