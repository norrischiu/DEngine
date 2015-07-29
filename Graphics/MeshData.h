// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include "D3D11Renderer.h" // Renderer instance
#include "../Object/Camera.h"

class MeshData
{
public:
	// Overload default constructor
	MeshData(const char* filename);

	void Update();

	void Render();

	int GetVertexNum()
	{
		return m_iNumVerts;
	}

	// Destructor
	~MeshData()
	{
		if (m_pVertexBuffer)
			m_pVertexBuffer->Release();
		if (m_pIndexBuffer)
			m_pIndexBuffer->Release();
		if (g_pConstantBuffer)
			g_pConstantBuffer->Release();
		if (inputLayout)
			inputLayout->Release();
	}

private:
	// Pointer to vertex buffer (included normal, texture coordinate)
	ID3D11Buffer* m_pVertexBuffer;	
	
	// Pointer to index buffer
	ID3D11Buffer* m_pIndexBuffer;

	// Pointer to input layout supply to IA
	ID3D11InputLayout* inputLayout;

	// Pointer to constant buffer
	ID3D11Buffer* g_pConstantBuffer;

	// Number of vertices
	unsigned int m_iNumVerts;

	// Number of indics
	unsigned int m_iNumIndics;

	// TEMP: camera
	Camera m_camera;
};

#endif