#ifndef POST_PROCESS_EFFECT_H_
#define POST_PROCESS_EFFECT_H_

namespace DE
{

// Engine include
class Texture;
class PSPerLightCBuffer;
class VSPerObjectCBuffer;
class MeshData;
class Material;

/*
*	CLASS: PostProcessEffect
*	PostProcessEffect is the interface to implement post-processing,
*	user can write their own HLSL shader code and define render pass 
*	here directly
*/
class PostProcessEffect
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	ShaderManager()
	*	This constructor will create the index and vertex buffer for a screen size
	*	quad for rendering post-processing effect in 2D, define custom render pass
	*	here
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	PostProcessEffect();

	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will call D3D draw call with the given render pass in order
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Render();

private:

	PSPerLightCBuffer*							m_pPSCBuffer;	// Constant buffer update facilities for each light
	VSPerObjectCBuffer*							m_pVSCBuffer;	// Constant buffer update facilities for each object	
	MeshData*									fullscreenQuadMesh;		// Full screen quad mesh
	Texture*									m_texture;	// Ping pong textures 1, for blurring
	Texture*									m_texture2;	// Ping pong textures 2, for blurring
};

};
#endif // !POST_PROCESS_EFFECT_H_
