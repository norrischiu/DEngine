#include "DEBUG_RENDERER.h"
#include "../GlobalInclude.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

DEBUG_RENDERER* DEBUG_RENDERER::m_pInstance = 0;

DEBUG_RENDERER::DEBUG_RENDERER()
	: m_vDbgMeshs(0)
	, m_vRenderFlags(0)
{
}

};