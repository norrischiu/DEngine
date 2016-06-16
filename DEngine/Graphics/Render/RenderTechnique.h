#ifndef RENDER_TECHNIQUE_H_
#define RENDER_TECHNIQUE_H_

// Engine include
#include "Utilities\MyArray.h"
#include "RenderPass.h"
#include "Memory\Handle.h"

namespace DE
{

/*
*	CLASS: RenderTechnique
*	RenderTechnique contains arbitrary number of render passes,
*	with at least one. This can be used to decided how many
*	render passes to use in different situation. For example,
*	according to the distance of the mesh from the camera
*/
class RenderTechnique
{

public:

	CUSTOM_MEMORY_DEFINE();

	/********************************************************************************
	*	--- Constructor:
	*	RenderTechnique()
	*	This constructor will construct a render technique with zero render pass
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	RenderTechnique()
		: m_vRenderPasses(0)
	{
	}

	/********************************************************************************
	*	--- Destructor:
	*	~RenderTechnique()
	*	This destructor will free the memory used by the render pass array
	********************************************************************************/
	~RenderTechnique()
	{
	}

	/********************************************************************************
	*	--- Function:
	*	BindPassAt(int)
	*	This function will bind the render pass at the given index to the renderer
	*
	*	--- Parameters:
	*	@ index: the index of the target render pass
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void BindPassAt(int index);

	/********************************************************************************
	*	--- Function:
	*	AddPass(RenderPass*)
	*	This function will add a render pass to this render technique, it follows a
	*	chronicle order
	*
	*	--- Parameters:
	*	@ pass: a render pass
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddPass(RenderPass* pass)
	{
		m_vRenderPasses.Add(pass);
	}

	MyArray<RenderPass*>					m_vRenderPasses;	// handle array of all render passes

};

};
#endif // RENDER_TECHNIQUE_H_

