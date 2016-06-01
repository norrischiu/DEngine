#ifndef RENDER_TECHNIQUE_H_
#define RENDER_TECHNIQUE_H_

#include "Utilities\MyArray.h"
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
		: m_vRenderPasses(0)
	{
	}

	void BindPassAt(int index);

	void AddPass(RenderPass* pass)
	{
		m_vRenderPasses.Add(pass);
	}

	MyArray<RenderPass*>					m_vRenderPasses;

	~RenderTechnique()
	{
	}

};

};
#endif // RENDER_TECHNIQUE_H_

