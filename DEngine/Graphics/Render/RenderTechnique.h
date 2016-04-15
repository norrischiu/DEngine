#ifndef RENDER_TECHNIQUE_H_
#define RENDER_TECHNIQUE_H_

#include <vector>
#include "RenderPass.h"
#include "Memory\Handle.h"

namespace DE
{

class RenderTechnique
{

public:

	CUSTOM_MEMORY_DEFINE();

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

	~RenderTechnique()
	{
	}

};

};
#endif // RENDER_TECHNIQUE_H_

