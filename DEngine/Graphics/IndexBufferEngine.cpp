#include "IndexBufferEngine.h"

namespace DE
{

IndexBufferEngine* IndexBufferEngine::m_pInstance;
unsigned int IndexBufferEngine::g_iCurrIndex = 0;

IndexBufferEngine* IndexBufferEngine::GetInstance() 
{
	if (!m_pInstance)
		m_pInstance = new IndexBufferEngine();
	return m_pInstance;
}

void IndexBufferEngine::DestructandCleanUp() 
{
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void* IndexBufferEngine::CreateBufferFromRawData(const unsigned int* pIndexData, const int m_iNumIndics) 
{
	HRESULT hr;

	// Create the index buffer
	D3D12Renderer* renderer = Renderer::GetInstance();
	ID3D12Resource* indexBuffer;
	int iBufferSize = sizeof(UINT) * m_iNumIndics;

	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	ID3D12Resource* iBufferUploadHeap;
	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&iBufferUploadHeap));

	// store vertex buffer in upload heap
	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = pIndexData;
	indexData.RowPitch = iBufferSize;
	indexData.SlicePitch = iBufferSize;

	UpdateSubresources(renderer->m_pCommandList, indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);
	renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	renderer->m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList };
	renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]++;
	hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence[renderer->m_iCurrFrameIndex], renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]);
	
	Renderer::GetInstance()->ResetCommandAllocatorAndList();

	return (void*)indexBuffer->GetGPUVirtualAddress();
}

void* IndexBufferEngine::CreateBuffer(const char * filename, unsigned int& indicsNum)
{
	unsigned int iNumTri;
	HRESULT hr;

	// Read vertices
	pFile = fopen(filename, "r");
	char c[256];
	fscanf(pFile, "%s", &c); // name
	fscanf(pFile, "%s", &c); // num of vertex of prim (must be 3 for now)
	fscanf(pFile, "%i", &iNumTri);
	indicsNum = iNumTri * 3;

	Handle hIndexData(sizeof(UINT) * indicsNum);
	FillIndexData(indicsNum, hIndexData);

	D3D12Renderer* renderer = Renderer::GetInstance();
	ID3D12Resource* indexBuffer;
	int iBufferSize = sizeof(UINT) * indicsNum;

	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	ID3D12Resource* iBufferUploadHeap;
	renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr,
		IID_PPV_ARGS(&iBufferUploadHeap));

	// store vertex buffer in upload heap
	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = hIndexData.Raw();
	indexData.RowPitch = iBufferSize;
	indexData.SlicePitch = iBufferSize;

	//renderer->WaitForPreviousFrame();

	UpdateSubresources(renderer->m_pCommandList, indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);
	renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	renderer->m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList };
	renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]++;
	hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence[renderer->m_iCurrFrameIndex], renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]);
	renderer->ResetCommandAllocatorAndList();

	hIndexData.Free();
	return (void*)indexBuffer->GetGPUVirtualAddress();
}

void IndexBufferEngine::FillIndexData(unsigned int indicsNum, Handle hIndexData)
{
	void* pIndexData = hIndexData.Raw();

	for (unsigned int i = 0; i < indicsNum; i++)
	{
		unsigned int x;
		fscanf(pFile, "%i", &x);
		((UINT*)pIndexData)[i] = x;
	}
	fclose(pFile);
}

};