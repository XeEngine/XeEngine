#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathMatrix.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe {
	namespace Graphics {
		interface IContext;

		//! \brief Draw sprites on an orthogonal space as fast as possible.
		class SpriteBatch : public IObject {
		protected:
			struct Info {
				Math::Vector2f Position[4];
				Math::Vector2f UV[4];
				Graphics::Color Color[4];
				float Depth;
				float Mode;
			};

			IContext* m_pContext;
		public:
			//! \brief Define how sprite are orderdetr
			enum SortMode {
				//! \brief Draw sprite in the same order of Draw calls.
				//! \details Sprites will be rendered when End is called.
				SortMode_Deferred,
				//! \brief Render sprite when Draw is called.
				/** \details This mode disallow batching and should be used
				 * only if state changes often during each draw call.
				 */
				SortMode_Immediate,
				SortMode_Texture,
				SortMode_BackToFront,
				SortMode_FrontToBack,
			};
			struct Properties {
				SortMode SortMode;

				static const Properties Default;
			};

			//! \param[in] pContext a valid context that will be used to draw graphics.
			SpriteBatch(IContext* pContext);

			//! \brief Get the context attached to current sprite batch.
			//! \param[out] ppContext cannot be nullptr.
			void GetGraphics(IContext** ppContext);

			//! \brief Inizialize a sprite block.
			void Begin(const Properties& properties = Properties::Default);

			//! \brief Render the initialized block.
			void End();
		};
	}
}