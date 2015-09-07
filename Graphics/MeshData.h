// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include "D3D11Renderer.h" // Renderer instance
#include "../Object/Camera.h"

enum eRenderType
{
	OUTLINE,
	STANDARD_MESH,
	STANDARD_MESH_WITH_SHADOW,
};

class MeshData
{
public:

	// Overload default constructor
	MeshData(const char* filename, int renderType);

	void Update();

	void Render();

	inline int GetVertexNum()
	{
		return m_iNumVerts;
	}

	// Read buffer
	void ReadBufferData(){};

	// Destructor
	~MeshData()
	{
		if (m_pVS)
			m_pVS->Release();
		if (m_pPS)
			m_pPS->Release();
		if (m_pIndexBuffer)
			m_pIndexBuffer->Release();
		if (g_pConstantBuffer)
			g_pConstantBuffer->Release();
		if (m_pInputLayout)
			m_pInputLayout->Release();
		if (m_pTexResourceView)
			m_pTexResourceView->Release();
		if (m_pSamplerState)
			m_pSamplerState->Release();
	}

private:

	// Pointer to complied to vertex shader
	ID3D11VertexShader*						m_pVS;
	
	// Pointer to complied to pixel shader
	ID3D11PixelShader*						m_pPS;

	// Pointer to index buffer
	ID3D11Buffer*							m_pIndexBuffer;

	// Pointer to input layout supply to IA
	ID3D11InputLayout*						m_pInputLayout;

	// Pointer to constant buffer
	ID3D11Buffer*							g_pConstantBuffer;

	// Pointer to resource view pass to shader
	ID3D11ShaderResourceView*				m_pTexResourceView;

	// Pointer to sampler state
	ID3D11SamplerState*						m_pSamplerState;

	// Primitive Topology
	unsigned int							m_iTopology;

	// Number of vertices
	unsigned int							m_iNumVerts;

	// Number of indics
	unsigned int							m_iNumIndics;

	// 
	unsigned int							m_iStartIndexLocation;

	//
	const char*								m_cFilename;

	// TEMP: camera
	Camera									m_camera;
};

#endif