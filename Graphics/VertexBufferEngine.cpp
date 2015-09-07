#include "VertexBufferEngine.h"
#include <stdio.h>
#include "VertexFormat.h"

// #define _CRT_SECURE_NO_WARNINGS

void VertexBufferEngine::CreateBuffer(const char * filename, int vertexFormat)
{
	int iNumVerts;
	HRESULT hr;
	ID3D11Buffer* pVertexBuffer;
	void* pVertexData = nullptr;
	UINT iDataSize;

	// Read vertices
	pFile = fopen(filename, "r");
	char c[256];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &iNumVerts);

	switch (vertexFormat)
	{
		case eVertexFormat::POSITION_TEXTURE:
			FillVertexData_POSITION_TEXTURE(iNumVerts, pVertexData);
			iDataSize = sizeof(Vertex1P1UV);
			break;
		case eVertexFormat::POSITION:
			FillVertexData_POSITION(iNumVerts, pVertexData);
			iDataSize = sizeof(Vertex1P1UV);
			break;
	}

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = pVertexData;
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = iDataSize * iNumVerts;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &pVertexBuffer);
	assert(hr == S_OK);
	UINT stride = iDataSize;
	UINT vertexOffset = 0;
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &vertexOffset);
	
	// Release
	if (pVertexBuffer)
		pVertexBuffer->Release();
}

void VertexBufferEngine::FillVertexData_POSITION(unsigned int vertsNum, void* &pVertexData)
{
	pVertexData = new Vertex1P[vertsNum]; // TODO: needs change memory allocation

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		fscanf(pFile, "%f", &z);
		((Vertex1P*) pVertexData)[i].m_pos = Vector3(x, y, z);
	}
	fclose(pFile);
}

void VertexBufferEngine::FillVertexData_POSITION_TEXTURE(unsigned int vertsNum, void* &pVertexData)
{
	pVertexData = new Vertex1P1UV[vertsNum]; // TODO: needs change memory allocation

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		fscanf(pFile, "%f", &z);
		((Vertex1P1UV*) pVertexData)[i].m_pos = Vector3(x, y, z);
	}
	fclose(pFile);

	// Read texture coordinates
	pFile = fopen("dragon_texture.bufa", "r");
	char c[256];
	fscanf(pFile, "%s",&c );
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		((Vertex1P1UV*) pVertexData)[i].m_UV[0] = x;
		((Vertex1P1UV*) pVertexData)[i].m_UV[1] = y;
	}
	fclose(pFile);
}
