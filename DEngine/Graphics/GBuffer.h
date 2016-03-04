#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <D3D11.h>
#include "Render\PSPerLightCBuffer.h"
#include "Render\VSPerObjectCBuffer.h"
#include "Render\Texture.h"

namespace DE
{

class MeshData;
class RenderPass;

class GBuffer
{

	/*
	*		|	R	|	G	|	B	|		A			|
	* RT0:	|	diffuse albedo.rgb	|  specular factor	|
	* RT1:	|		normal.xyz		|     shininess		|
	*/

public:

	// Default constructor
	GBuffer();

	void Render();

private:

	static const unsigned int					RT_NUM = 3;

	// Pointer to vertex buffer
	ID3D11Buffer*								m_pPointLightVB;
	ID3D11Buffer*								m_pSpotLightVB;

	// Pointer to index buffer
	ID3D11Buffer*								m_pPointLightIB;
	ID3D11Buffer*								m_pSpotLightIB;

	// Constant buffer update facilities
	PSPerLightCBuffer*							m_pPSCBuffer;
	VSPerObjectCBuffer*							m_pVSCBuffer;

	MeshData*									pointLightMesh;
	MeshData*									spotLightMesh;

	RenderPass*									StencilingPass;
	RenderPass*									LightingPass;
};

};

#endif // !GBUFFER_H_
