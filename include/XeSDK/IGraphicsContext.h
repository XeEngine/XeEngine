
#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeMathVector3.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeGraphicsColor.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/XeMathRectangle.h>

namespace Xe { namespace Graphics {
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

	enum Primitive
	{
		Primitive_PointList,
		Primitive_LineList,
		Primitive_LineStrip,
		Primitive_TriangleList,
		Primitive_TriangleStrip
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

	struct Vertex {
		Xe::Math::Vector3f Position;
		Xe::Math::Vector3f Texture;
		Xe::Graphics::Color Color;
	};

	//! \brief describe capabilities of specified context
	struct Capabilities
	{
		int MaxTexture1DSize;
		int MaxTexture1DArrayLength;
		int MaxTexture2DSize;
		int MaxTexture2DArrayLength;
		int MaxTexture3DSize;
		int MaxTextureCubeSize;
		int MaxTextureUnits;
		int MaxAntisotropy;
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

	struct BufferDesc
	{
		UsageType Usage;

		u32 Length;

		BufferType Type;
	};

	//! \brief State of depth and stencil associated to a context
	interface IDepthStencilState : public IObject
	{
	};

	interface IContext : public IObject
	{
		//! \brief used to syncronize swap buffers with v-blank
		enum VBlankSync
		{
			VBlank_DoNotWait,
			VBlank_Wait,
		};

		virtual void GetCapabilities(Capabilities& capabilities) = 0;

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

		virtual void GetRenderingSurface(ISurface** ppSurface) = 0;
		virtual void SetRenderingSurface(ISurface* surface) = 0;

		virtual bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) = 0;
		virtual void SelectDepthStencilState(IDepthStencilState *depthStencilState) = 0;

		virtual bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData) = 0;
		virtual void GetVertexBuffer(IBuffer** ppBuffer) = 0;
		virtual void SetVertexBuffer(IBuffer* pBuffer) = 0;

		virtual void GetIndexBuffer(IBuffer** ppBuffer) = 0;
		virtual void SetIndexBuffer(IBuffer* pBuffer) = 0;

		virtual const Color &GetClearColor() const = 0;
		virtual void SetClearColor(const Color &color) = 0;
		virtual float GetClearDepth() const = 0;
		virtual void SetClearDepth(float depth) = 0;
		virtual int GetClearStencil() const = 0;
		virtual void SetClearStencil(int stencil) = 0;

		virtual bool IsScissorEnabled() const = 0;

		virtual void SetScissorEnabled(bool enabled) = 0;

		//! \brief Get the current scissor rectangle.
		virtual const Xe::Math::Rectangle<int> GetScissor() const = 0;

		//! \brief Set the scissor rectangle bound.
		//! \param scissor Scissor rectangle, where each value must be positive.
		/** \details When a scissor rectangle is set, everything pixel outside will be discarded.
		 * The default value is an empty rectangle, where all the values are 0
		 */
		virtual void SetScissor(const Xe::Math::Rectangle<int>& scissor) = 0;

		//! \brief clear selected rendering buffer
		//! \param[in] clearmask mask of ClearMask 
		//! \sa ClearMask
		virtual void Clear(svar clearmask) = 0;

		virtual void Draw(Primitive primitive, u32 count, u32 start = 0) = 0;
		virtual void DrawIndexed(Primitive primitive, u32 count, u32 start = 0) = 0;

		virtual void SetInternalResolution(const Size& size) = 0;
		//! \brief present the changes to the visible screen buffer
		//! \param[in] vblank describe how the swapped buffer will be syncronized with v-blank
		virtual void SwapBuffers(VBlankSync vblank) = 0;
	};
} }