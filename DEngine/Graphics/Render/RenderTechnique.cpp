#include "RenderTechnique.h"

namespace DE
{

void RenderTechnique::BindPassAt(int index)
{
	m_vRenderPasses[index]->BindToRenderer();
}

};