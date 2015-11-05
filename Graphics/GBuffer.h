#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <D3D11.h>

class GBuffer
{
public:

	// Default constructor
	GBuffer();

	void Render();
	
private:
	
	static const unsigned int					RT_NUM = 2;

	// Pointer to vertex buffer
	ID3D11Buffer*								m_pVertexBuffer;

	// Pointer to index buffer
	ID3D11Buffer*								m_pIndexBuffer;

	// Pointer to complied to vertex shader
	ID3D11VertexShader*							m_pVS;

	// Pointer to complied to pixel shader
	ID3D11PixelShader*							m_pPS;

	// Pointer to input layout supply to IA
	ID3D11InputLayout*							m_pInputLayout;

	// Pointer to resource view array pass to shader
	ID3D11ShaderResourceView*					m_pTexResourceView[RT_NUM];

	// Pointer to sampler state
	ID3D11SamplerState*							m_pSamplerState;

	// Pointer to constant buffer
	ID3D11Buffer*								m_pConstantBuffer;

	// Number of indics
	unsigned int								m_iNumIndics;
		
};

#endif // !GBUFFER_H_
