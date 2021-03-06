#pragma once
#include <XeSDK/XeError.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeMathVector2.h>

namespace Xe {
	namespace Core {
		interface IFrameView;
	}

	//! \brief Graphics and drawing management
	namespace Graphics {
		interface IContext;
		typedef Math::Vector2i Position;
		typedef Math::Vector2i Size;

		//! \brief Specify a render engine used to draw on screen.
		//! \warning DEPRECATED!!!
		enum RenderEngine {
			//! \brief Defaut value for rendering.
			//! \sa GetDefaultRenderEngine
			RenderEngine_Default = 0,

			//! \brief A null device used when rendering is not needed.
			RenderEngine_Null = 1,

			//! \brief Use Direct3D9 as rendering engine
			//! \details Used starting from Windows 98 and Xbox 360
			RenderEngine_D3D9 = 2,

			//! \brief Use Direct3D11 as rendering engine
			//! \details Used starting from Windows Vista SP2 and XBox One
			RenderEngine_D3D11 = 3,

			//! \brief Use Direct3D12 as rendering engine
			//! \details Used starting from Windows 10
			RenderEngine_D3D12 = 4,

			//! \brief Use OpenGL as rendering engine
			/** \brief This uses most recent OpenGL version when possible.
			 * Minimum supported version is OpenGL 2.0 and the greater
			 * capability used is from OpenGL 3.1.
			 */
			RenderEngine_OpenGL = 5,
		};

		//! \brief Define the properties during Context initialization
		struct ContextInitDesc
		{
			//! \brief What video-card to use.
			//! \details Specify 0 to use default video-card.
			uvar VideoCardIndex;

			//\ brief Specify what display to use.
			//! \details Specify 0 to use default display.
			uvar DisplayIndex;

			//! brief Set the frame which the context should write on
			Xe::Core::IFrameView* FrameView;

			ContextInitDesc() :
				VideoCardIndex(0),
				DisplayIndex(0),
				FrameView(nullptr)
			{ }
		};

		//! \warning DEPRECATED; for backward compatibility only.
		bool Create(IContext **context, RenderEngine renderEngine, const ContextInitDesc& properties = ContextInitDesc());

		//! \warning DEPRECATED; for backward compatibility only.
		bool Create(IContext **context, ctstring driverName, const ContextInitDesc& properties = ContextInitDesc());
	}
}