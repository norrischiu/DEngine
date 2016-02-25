#ifndef POST_PROCESS_EFFECT_H_
#define POST_PROCESS_EFFECT_H_

// Engine include
class Texture;
class PSPerLightCBuffer;
class VSPerObjectCBuffer;
class MeshData;
class Material;
class ID3D11Buffer;

class PostProcessEffect
{

public:

	// Default constructor
	PostProcessEffect();

	void Render();

private:


	// Pointer to vertex buffer
	ID3D11Buffer*								m_pQuadVB;

	// Pointer to index buffer
	ID3D11Buffer*								m_pQuadIB;

	// Constant buffer update facilities
	PSPerLightCBuffer*							m_pPSCBuffer;
	VSPerObjectCBuffer*							m_pVSCBuffer;

	// Full screen quad mesh
	MeshData*									fullscreenQuadMesh;

	// Ping pong textures
	Texture*									m_texture;
	Texture*									m_texture2;
};

#endif // !POST_PROCESS_EFFECT_H_
