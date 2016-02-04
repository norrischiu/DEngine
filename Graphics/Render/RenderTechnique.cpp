#include "RenderTechnique.h"

void RenderTechnique::BindPassAt(int index)
{
	m_vRenderPasses[index]->BindToRenderer();
}
