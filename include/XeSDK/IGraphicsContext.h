#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeGraphicsColor.h>

#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsTilemap.h>

namespace Xe {
	namespace Graphics {
		//! \brief type of clearing function for selected rendering buffer
		//! \details values can be masked
		enum ClearMask
		{
			//! \brief don't perform any clear operation
			Clear_None = 0,
			//! \brief clear color's buffer
			Clear_Color = 1,
			//! \brief clear z-buffer
			Clear_Depth = 2,
			//! \brief clear stencil
			Clear_Stencil = 4,
		};
		//! \brief comparison operators
		enum Comparison
		{
			//! \brief comparison never pass
			Comparison_Never,
			//! \brief src < dst
			Comparison_Less,
			//! \brief src == dst
			Comparison_Equal,
			//! \brief src <= dst
			Comparison_LessEqual,
			//! \brief src > dst
			Comparison_Greater,
			//! \brief src != dst
			Comparison_NotEqual,
			//! \brief src >= dst
			Comparison_GreaterEqual,
			//! \brief comparison never fails
			Comparison_Always,
		};

		//! \brief describe capabilities of specified context
		struct Capabilities {
			svar MaxTextureUnits;
			svar MaxTextureSize;
			Capabilities();
		};
		//! \brief state description of a depth & stencil state
		struct DepthStencilStateDesc
		{
			//! \brief Enable depth test to discard or allow a pixel
			//! \details Default value is true
			bool depthTestEnabled;
			//! \brief Enable writing into depth buffer
			//! \details Default value is true
			bool depthWriteEnabled;
			//! \brief Operator used for compare two depths
			//! \details Default value is Comparison_LessEqual
			Comparison depthComparison;
			//! \brief Enable stencil test
			//! \details Default value is false
			bool stencilTestEnabled;
			//! \details Default value is 0xFF
			uvar stencilMask;
			//! \details Default value is Comparison_Always
			Comparison stencilFrontFaceComp;
			//! \details Default value is Comparison_Always
			Comparison stencilBackFaceComp;

			static DepthStencilStateDesc Default();
		};

		//! \brief State of depth and stencil associated to a context
		class IDepthStencilState : public IObject
		{
		public:
			//! \brief interface id
			static const UID ID = 0x79a9356601dc4475L;
		};

		class IDrawing2d;
		class IContext : public IObject {
		public:
			static const UID ID = 0x7c368cde9f314894ULL;

			//! \brief used to syncronize swap buffers with v-blank
			enum VBlankSync {
				VBlank_DoNotWait,
				VBlank_Wait,
			};

			virtual void GetDrawing(IDrawing2d **ppDrawing) = 0;
			virtual void CreateTilemap(ITilemap **ppTilemap) = 0;

			///////////////////////////////////////////////////////////////////
			// States

			//! \brief create a 2D surface where image data can be stored
			//! \param[out] surface created; nullptr if function returns false
			//! \param[in] type of surface to create
			//! \param[in] size in pixel of surface to create
			//! \param[in] format of colors of the surface; not all formats are supported
			//! \param[in] datadesc description of data that will be uploaded
			virtual bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& datadesc = DataDesc()) = 0;
			//! \brief select a surface to use during drawing
			//! \param[in] surface to select; it can be nullptr
			//! \param[in] index slot index where the surface will be selected
			/** \details When a surface is selected, it's assigned to the
			 * specified slot and every slot can store only one surface at time.
			 */
			virtual void SelectSurface(ISurface *surface, svar slot) = 0;

			virtual bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) = 0;
			virtual void SelectDepthStencilState(IDepthStencilState *depthStencilState) = 0;

			virtual const Color &GetClearColor() const = 0;
			virtual void SetClearColor(const Color &color) = 0;
			virtual float GetClearDepth() const = 0;
			virtual void SetClearDepth(float depth) = 0;
			virtual int GetClearStencil() const = 0;
			virtual void SetClearStencil(int stencil) = 0;
			//! \brief clear selected rendering buffer
			//! \param[in] clearmask mask of ClearMask 
			//! \sa ClearMask
			virtual void Clear(svar clearmask) = 0;

			virtual void SetInternalResolution(const Size& size) = 0;
			//! \brief present the changes to the visible screen buffer
			//! \param[in] vblank describe how the swapped buffer will be syncronized with v-blank
			virtual void SwapBuffers(VBlankSync vblank) = 0;
		};
	}
}