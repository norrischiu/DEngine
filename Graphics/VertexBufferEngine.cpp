#include "VertexBufferEngine.h"
#include <stdio.h>
#include "VertexFormat.h"

#define C_STR(string, text)\
		(string + text).c_str()

// #define _CRT_SECURE_NO_WARNINGS

void* VertexBufferEngine::CreateBuffer(const char * filename, int vertexFormat, unsigned int& stride)
{
	std::string sFileNmae(filename);
	int iNumVerts;
	HRESULT hr;
	ID3D11Buffer* pVertexBuffer;
	void* pVertexData = nullptr;

	// Read vertices
	pFile = fopen(C_STR(sFileNmae, "_vertex.bufa"), "r");
	char c[256];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &iNumVerts);

	switch (vertexFormat)
	{
		case eVertexFormat::POSITION_TEXTURE:
			FillVertexData_POSITION_TEXTURE(filename, iNumVerts, pVertexData);
			stride = sizeof(Vertex1P1UV);
			break;
		case eVertexFormat::POSITION:
			FillVertexData_POSITION(filename, iNumVerts, pVertexData);
			stride = sizeof(Vertex1P1UV);
			break;
		case eVertexFormat::POSITION_NORMAL_TEXTURE:
			FillVertexData_POSITION_NORMAL_TEXTURE(filename, iNumVerts, pVertexData);
			stride = sizeof(Vertex1P1N1UV);
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
	vertexBufferDesc.ByteWidth = stride * iNumVerts;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &pVertexBuffer);
	assert(hr == S_OK);
	
	return pVertexBuffer;
}

void VertexBufferEngine::FillVertexData_POSITION(const char* filename, unsigned int vertsNum, void* &pVertexData)
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

void VertexBufferEngine::FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData)
{
	std::string sFileNmae(filename);
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
	pFile = fopen(C_STR(sFileNmae, "_texture.bufa"), "r");
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

void VertexBufferEngine::FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, void *& pVertexData)
{
	std::string sFileNmae(filename);
	pVertexData = new Vertex1P1N1UV[vertsNum]; // TODO: needs change memory allocation

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		fscanf(pFile, "%f", &z);
		((Vertex1P1N1UV*)pVertexData)[i].m_pos = Vector3(x, y, z);
	}
	fclose(pFile);

	// Read texture coordinates
	pFile = fopen(C_STR(sFileNmae, "_texture.bufa"), "r");
	char c[256];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		((Vertex1P1N1UV*)pVertexData)[i].m_UV[0] = x;
		((Vertex1P1N1UV*)pVertexData)[i].m_UV[1] = y;
	}
	fclose(pFile);

	// Read precalcuated normal
	pFile = fopen(C_STR(sFileNmae, "_normal.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f", &x);
		fscanf(pFile, "%f", &y);
		fscanf(pFile, "%f", &z);
		((Vertex1P1N1UV*)pVertexData)[i].m_norm = Vector3(x, y, z);
	}
	fclose(pFile);
}
