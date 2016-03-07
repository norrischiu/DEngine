#include "DEBUG_RENDERER.h"

namespace DE
{

DEBUG_RENDERER* DEBUG_RENDERER::m_pInstance = 0;

DEBUG_RENDERER::DEBUG_RENDERER()
{
	m_m2DProjection = Matrix4::OrthographicProjection(1024, 768, 0.0f, 1.0f);
}

};