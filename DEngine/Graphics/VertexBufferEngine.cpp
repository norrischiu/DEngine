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

void* VertexBufferEngine::CreateBuffer(const char * filename, int vertexFormat, unsigned int& stride, unsigned int& bufferSize)
{
	std::string sFileNmae(filename);
	int iNumVerts;
	HRESULT hr;
	Handle hVertexData;

	// Read vertices
	pFile = fopen(C_STR(sFileNmae, "_vertex.bufa"), "r");
	char c[256];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &iNumVerts);

	switch (vertexFormat)
	{
	case eVertexFormat::POSITION_TEXTURE:
		hVertexData.Set(sizeof(Vertex1P1UV) * iNumVerts);
		FillVertexData_POSITION_TEXTURE(filename, iNumVerts, hVertexData);
		stride = sizeof(Vertex1P1UV);
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

	bufferSize = stride * iNumVerts;
	ID3D12Resource* vertexBuffer;
	Renderer* renderer = Renderer::GetInstance();

	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));
	vertexBuffer->SetName(L"Vertex Buffer Resource default Heap");

	ID3D12Resource* vBufferUploadHeap;
	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vBufferUploadHeap));
	vBufferUploadHeap->SetName(L"Vertex Buffer Resource upload Heap");

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = hVertexData.Raw();
	vertexData.RowPitch = bufferSize;
	vertexData.SlicePitch = bufferSize;

	UpdateSubresources(renderer->m_pCommandList, vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);
	renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	renderer->m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList };
	renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]++;
	hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence[renderer->m_iCurrFrameIndex], renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]);
	renderer->ResetCommandAllocatorAndList();

	hVertexData.Free();
	return (void*) vertexBuffer->GetGPUVirtualAddress();
}

void VertexBufferEngine::DestructandCleanUp() 
{
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void* VertexBufferEngine::CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize, bool streamOut)
{
	HRESULT hr;

	int bufferSize = iDataSize * iNumVerts;
	ID3D12Resource* vertexBuffer;
	Renderer* renderer = Renderer::GetInstance();

	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	ID3D12Resource* vBufferUploadHeap;
	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vBufferUploadHeap));

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = pVertexData;
	vertexData.RowPitch = bufferSize;
	vertexData.SlicePitch = bufferSize;

	UpdateSubresources(renderer->m_pCommandList, vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);
	renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	renderer->m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList };
	renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]++;
	hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence[renderer->m_iCurrFrameIndex], renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]);

	Renderer::GetInstance()->ResetCommandAllocatorAndList();

	return (void*)vertexBuffer->GetGPUVirtualAddress();
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
		((Vertex1P1UV*)pVertexData)[i].m_pos = Vector3(x, y, z);
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
		((Vertex1P1UV*)pVertexData)[i].m_UV[0] = x;
		((Vertex1P1UV*)pVertexData)[i].m_UV[1] = y;
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