#include "pch.h"
#include "ContextGL.Common.h"

using namespace Xe::Graphics;

bool CContextGLCommon::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc)
{
	*depthStencilState = new CDepthStencilState(this, desc);
	return *depthStencilState != nullptr;
}

void CContextGLCommon::SelectDepthStencilState(IDepthStencilState *depthStencilState)
{
	m_DepthStencilState = dynamic_cast<CDepthStencilState*>(depthStencilState);
	// TODO set the proper state
}

CContextGLCommon::CDepthStencilState::CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc) :
	m_context(context)
{
	m_context->AddRef();
}

CContextGLCommon::CDepthStencilState::~CDepthStencilState()
{
	m_context->Release();
}
