#include "pch.h"
#include "ContextGl.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>

using namespace Xe::Graphics;

CContextGL::CContextGL(Xe::Core::IFrameView *pView) :
	CContextGLCommon(pView),
	m_Size(800, 480)
{
}

CContextGL::~CContextGL()
{
}

bool CContextGL::Initialize(const ContextInitDesc& properties)
{
	if (CContextGLCommon::Initialize(properties))
	{
		return true;
	}

	return false;
}

void CContextGL::GetCapabilities(Capabilities& capabilities)
{
	capabilities.MaxTexture1DSize = 0;
	capabilities.MaxTexture1DArrayLength = 0;
	capabilities.MaxTexture2DSize = 0;
	capabilities.MaxTexture2DArrayLength = 0;
	capabilities.MaxTexture3DSize = 0;
	capabilities.MaxTextureCubeSize = 0;
	capabilities.MaxTextureUnits = 0;
	capabilities.MaxAntisotropy = 0;
}

void CContextGL::SetClearDepth(float depth)
{
	SetClearDepthSub(depth);
	glClearDepth((GLclampd)depth);
	//glClearDepthf((GLclampf)depth); // GLES
}

void CContextGL::SwapBuffers(VBlankSync sync)
{
	//m_Drawing->Flush();
	SwapBuffersSub(sync);
}