#ifndef RENDER_TECHNIQUE_H_
#define RENDER_TECHNIQUE_H_

#include <vector>
#include "RenderPass.h"

class RenderTechnique
{

public:

	// Empty constructor
	RenderTechnique() 
	{
	}

	void BindPassAt(int index);

	void AddPass(RenderPass* pass)
	{
		m_vRenderPasses.push_back(pass);
	}

	std::vector<RenderPass*>				m_vRenderPasses;

};

#endif // RENDER_TECHNIQUE_H_

