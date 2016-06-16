#ifndef GBUFFER_H_
#define GBUFFER_H_

// D3D11 include
#include <D3D11.h>

// Engine include
#include "Render\PSPerLightCBuffer.h"
#include "Render\VSPerObjectCBuffer.h"
#include "Render\Texture.h"

namespace DE
{

// Engine include
class MeshData;
class RenderPass;

/*
*	CLASS: GBuffer
*	GBuffer controls the rendering to screen after the attributes 
*	are written to gbuffer, optimization with light volume 
*	rendering is applied here. The gbuffer layout as follows:
*	
*			|	R	|	G	|	B	|		A			|
*	RT0:	|	diffuse albedo.rgb	|  specular factor	|
*	RT1:	|		normal.xyz		|     shininess		|
*	DSV:	|		depth			|     stencil		|
*
*/
class GBuffer
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	GBuffer()
	*	This constructor will create the default render pass for second pass of
	*	deferred shading
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	GBuffer();

	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will render the gbuffer using light volume
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Render();

private:

	static const unsigned int					RT_NUM = 3;		// number of render target in gbuffer
	PSPerLightCBuffer							m_pPSCBuffer;	// Constant buffer update facilities for each light
	VSPerObjectCBuffer							m_pVSCBuffer;	// Constant buffer update facilities for each object
	MeshData*									pointLightMesh;		// default point light mesh, which is a box
	MeshData*									spotLightMesh;		// default point light mesh, which is a prism
	RenderPass*									StencilingPass;		// render pass for pixel marking optimization
	RenderPass*									LightingPass;		// render pass for final light pass
};

};

#endif // !GBUFFER_H_
