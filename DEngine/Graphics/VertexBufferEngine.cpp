#include "VertexBufferEngine.h"
#include <stdio.h>
#include "VertexFormat.h"
#include "Utilities\Geometry.h"

#define C_STR(string, text)\
	(string + text).c_str()

namespace DE
{

VertexBufferEngine* VertexBufferEngine::m_pInstance;

VertexBufferEngine* VertexBufferEngine::GetInstance() 
{
	if (!m_pInstance)
		m_pInstance = new VertexBufferEngine();
	return m_pInstance;
}

void* VertexBufferEngine::CreateBuffer(const char * filename, int vertexFormat, unsigned int& stride)
{
	std::string sFileNmae(filename);
	int iNumVerts;
	HRESULT hr;
	ID3D11Buffer* pVertexBuffer;
	Handle hVertexData;

	// Read vertices
	pFile = fopen(C_STR(sFileNmae, "_vertex.bufa"), "r");
	char c[256];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &iNumVerts);

	switch (vertexFormat)
	{
	case eVertexFormat::POSITION_TEXTURE:
		hVertexData.Set(sizeof(vertex1P1UV) * iNumVerts);
		FillVertexData_POSITION_TEXTURE(filename, iNumVerts, hVertexData);
		stride = sizeof(vertex1P1UV);
		break;
	case eVertexFormat::POSITION:
		hVertexData.Set(sizeof(Vertex1P) * iNumVerts);
		FillVertexData_POSITION(filename, iNumVerts, hVertexData);
		stride = sizeof(Vertex1P);
		break;
	case eVertexFormat::POSITION_NORMAL_TEXTURE:
		hVertexData.Set(sizeof(Vertex1P1N1UV) * iNumVerts);
		FillVertexData_POSITION_NORMAL_TEXTURE(filename, iNumVerts, hVertexData);
		stride = sizeof(Vertex1P1N1UV);
		break;
	case eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE:
		hVertexData.Set(sizeof(Vertex1P1N1T1UV) * iNumVerts);
		FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(filename, iNumVerts, hVertexData);
		stride = sizeof(Vertex1P1N1T1UV);
		break;
	case eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS:
		hVertexData.Set(sizeof(Vertex1P1N1T1UV4J) * iNumVerts);
		FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS(filename, iNumVerts, hVertexData);
		stride = sizeof(Vertex1P1N1T1UV4J);
		break;
	}

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = hVertexData.Raw();
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
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &pVertexBuffer);
	assert(hr == S_OK);

	hVertexData.Free();
	return pVertexBuffer;
}

void VertexBufferEngine::DestructandCleanUp() 
{
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

ID3D11Buffer* VertexBufferEngine::CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize, bool streamOut)
{
	HRESULT hr;
	ID3D11Buffer* pVertexBuffer;

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = pVertexData;
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (streamOut)
	{
		vertexBufferDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
	}
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = iDataSize * iNumVerts;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &pVertexBuffer);
	assert(hr == S_OK);

	return pVertexBuffer;
}

void VertexBufferEngine::FillVertexData_POSITION(const char* filename, unsigned int vertsNum, Handle hVertexData)
{
	void* pVertexData = hVertexData.Raw();

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P*)pVertexData)[i].m_pos = Vector3(x, y, z);
		m_vMaxXYZ.SetX(x > m_vMaxXYZ.GetX() ? x : m_vMaxXYZ.GetX());
		m_vMaxXYZ.SetY(y > m_vMaxXYZ.GetY() ? y : m_vMaxXYZ.GetY());
		m_vMaxXYZ.SetZ(z > m_vMaxXYZ.GetZ() ? z : m_vMaxXYZ.GetZ());
		m_vMinXYZ.SetX(x < m_vMinXYZ.GetX() ? x : m_vMinXYZ.GetX());
		m_vMinXYZ.SetY(y < m_vMinXYZ.GetY() ? y : m_vMinXYZ.GetY());
		m_vMinXYZ.SetZ(z < m_vMinXYZ.GetZ() ? z : m_vMinXYZ.GetZ());
	}
	fclose(pFile);
}

void VertexBufferEngine::FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData)
{
	std::string sFileNmae(filename);
	void* pVertexData = hVertexData.Raw();

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((vertex1P1UV*)pVertexData)[i].m_pos = Vector3(x, y, z);
		m_vMaxXYZ.SetX(x > m_vMaxXYZ.GetX() ? x : m_vMaxXYZ.GetX());
		m_vMaxXYZ.SetY(y > m_vMaxXYZ.GetY() ? y : m_vMaxXYZ.GetY());
		m_vMaxXYZ.SetZ(z > m_vMaxXYZ.GetZ() ? z : m_vMaxXYZ.GetZ());
		m_vMinXYZ.SetX(x < m_vMinXYZ.GetX() ? x : m_vMinXYZ.GetX());
		m_vMinXYZ.SetY(y < m_vMinXYZ.GetY() ? y : m_vMinXYZ.GetY());
		m_vMinXYZ.SetZ(z < m_vMinXYZ.GetZ() ? z : m_vMinXYZ.GetZ());
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
		fscanf(pFile, "%f %f", &x, &y);
		((vertex1P1UV*)pVertexData)[i].m_UV[0] = x;
		((vertex1P1UV*)pVertexData)[i].m_UV[1] = y;
	}
	fclose(pFile);
}

void VertexBufferEngine::FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData)
{
	std::string sFileNmae(filename);
	void* pVertexData = hVertexData.Raw();

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1UV*)pVertexData)[i].m_pos = Vector3(x, y, z);
		m_vMaxXYZ.SetX(x > m_vMaxXYZ.GetX() ? x : m_vMaxXYZ.GetX());
		m_vMaxXYZ.SetY(y > m_vMaxXYZ.GetY() ? y : m_vMaxXYZ.GetY());
		m_vMaxXYZ.SetZ(z > m_vMaxXYZ.GetZ() ? z : m_vMaxXYZ.GetZ());
		m_vMinXYZ.SetX(x < m_vMinXYZ.GetX() ? x : m_vMinXYZ.GetX());
		m_vMinXYZ.SetY(y < m_vMinXYZ.GetY() ? y : m_vMinXYZ.GetY());
		m_vMinXYZ.SetZ(z < m_vMinXYZ.GetZ() ? z : m_vMinXYZ.GetZ());
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
		fscanf(pFile, "%f %f", &x, &y);
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
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1UV*)pVertexData)[i].m_norm = Vector3(x, y, z);
	}
	fclose(pFile);
}

void VertexBufferEngine::FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData)
{
	std::string sFileNmae(filename);
	void* pVertexData = hVertexData.Raw();

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV*)pVertexData)[i].m_pos = Vector3(x, y, z);
		m_vMaxXYZ.SetX(x > m_vMaxXYZ.GetX() ? x : m_vMaxXYZ.GetX());
		m_vMaxXYZ.SetY(y > m_vMaxXYZ.GetY() ? y : m_vMaxXYZ.GetY());
		m_vMaxXYZ.SetZ(z > m_vMaxXYZ.GetZ() ? z : m_vMaxXYZ.GetZ());
		m_vMinXYZ.SetX(x < m_vMinXYZ.GetX() ? x : m_vMinXYZ.GetX());
		m_vMinXYZ.SetY(y < m_vMinXYZ.GetY() ? y : m_vMinXYZ.GetY());
		m_vMinXYZ.SetZ(z < m_vMinXYZ.GetZ() ? z : m_vMinXYZ.GetZ());
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
		fscanf(pFile, "%f %f", &x, &y);
		((Vertex1P1N1T1UV*)pVertexData)[i].m_UV[0] = x;
		((Vertex1P1N1T1UV*)pVertexData)[i].m_UV[1] = y;
	}
	fclose(pFile);

	// Read precalcuated normal
	pFile = fopen(C_STR(sFileNmae, "_normal.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV*)pVertexData)[i].m_norm = Vector3(x, y, z, 0);
	}
	fclose(pFile);

	// Read precalculated tangent
	pFile = fopen(C_STR(sFileNmae, "_tangent.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV*)pVertexData)[i].m_tangent = Vector3(x, y, z, 0);
	}
	fclose(pFile);
}

void VertexBufferEngine::FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS(const char* filename, unsigned int vertsNum, Handle hVertexData)
{
	std::string sFileNmae(filename);
	void* pVertexData = hVertexData.Raw();

	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_pos = Vector3(x, y, z);
		m_vMaxXYZ.SetX(x > m_vMaxXYZ.GetX() ? x : m_vMaxXYZ.GetX());
		m_vMaxXYZ.SetY(y > m_vMaxXYZ.GetY() ? y : m_vMaxXYZ.GetY());
		m_vMaxXYZ.SetZ(z > m_vMaxXYZ.GetZ() ? z : m_vMaxXYZ.GetZ());
		m_vMinXYZ.SetX(x < m_vMinXYZ.GetX() ? x : m_vMinXYZ.GetX());
		m_vMinXYZ.SetY(y < m_vMinXYZ.GetY() ? y : m_vMinXYZ.GetY());
		m_vMinXYZ.SetZ(z < m_vMinXYZ.GetZ() ? z : m_vMinXYZ.GetZ());
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
		fscanf(pFile, "%f %f", &x, &y);
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_UV[0] = x;
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_UV[1] = y;
	}
	fclose(pFile);

	// Read precalcuated normal
	pFile = fopen(C_STR(sFileNmae, "_normal.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_norm = Vector3(x, y, z, 0);
	}
	fclose(pFile);

	// Read precalculated tangent
	pFile = fopen(C_STR(sFileNmae, "_tangent.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float x, y, z;
		fscanf(pFile, "%f %f %f", &x, &y, &z);
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_tangent = Vector3(x, y, z, 0);
	}
	fclose(pFile);

	// Read skin weighting and joint index
	pFile = fopen(C_STR(sFileNmae, "_weight.bufa"), "r");
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &vertsNum);
	for (unsigned int i = 0; i < vertsNum; i++)
	{
		float vec[4];
		int index;
		for (int j = 0; j < 4; j++)
		{
			fscanf(pFile, "%i", &index);
			((Vertex1P1N1T1UV4J*)pVertexData)[i].m_jointIndex[j] = index;
			fscanf(pFile, "%f", &vec[j]);
		}
		((Vertex1P1N1T1UV4J*)pVertexData)[i].m_skinWeight = Vector3(vec[0], vec[1], vec[2], vec[3]);
	}
	fclose(pFile);
}

};