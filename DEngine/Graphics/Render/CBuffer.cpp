#include "CBuffer.h"
#include "Math\simdmath.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

ID3D12Resource*	CBuffer::CBV_UPLOAD_HEAP = nullptr;
UINT CBuffer::CBV_UPLOAD_HEAP_CURR_OFFSET = 0;
void* CBuffer::CBV_UPLOAD_HEAP_BEGIN = nullptr;

void CBuffer::InitializeCBVUploadHeap(D3DRenderer* renderer)
{
	((D3D12Renderer*)renderer)->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(64 * 1024),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&CBV_UPLOAD_HEAP));
	CBV_UPLOAD_HEAP->SetName(L"Constant Buffer Upload Resource Heap");

	CBV_UPLOAD_HEAP_CURR_OFFSET = 0;

	CD3DX12_RANGE readRange(0, 0);
	CBV_UPLOAD_HEAP->Map(0, &readRange, reinterpret_cast<void**>(&CBV_UPLOAD_HEAP_BEGIN));
}

CBuffer::CBuffer(int type, size_t size)
: m_iType(type)
, m_iSize(size)
{
	HRESULT hr;

#ifdef D3D12
	//D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());

	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	//cbvDesc.BufferLocation = CBV_UPLOAD_HEAP->GetGPUVirtualAddress() + CBV_UPLOAD_HEAP_CURR_OFFSET;
	//cbvDesc.SizeInBytes = (size + 255) & ~255;    // CB size is required to be 256-byte aligned.
	//renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_CbvSrvUavHeapHandle);
	//
	//// offset upload heap pointer and descriptor pointer
	//m_CPUHandle = renderer->m_CbvSrvUavHeapHandle;
	//renderer->m_CbvSrvUavHeapHandle.Offset(1, renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	//m_iUploadHeapOffset = CBV_UPLOAD_HEAP_CURR_OFFSET;
	//CBV_UPLOAD_HEAP_CURR_OFFSET += cbvDesc.SizeInBytes;

	m_Memory._data = malloc(size); 

#elif defined D3D11
	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	constBufferDesc.ByteWidth = size;
	m_Memory._data = malloc(size);
	constResourcesData.pSysMem = m_Memory._data;
	hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_Memory._buffer);
	assert(hr == S_OK);
#endif
}

void CBuffer::BindToRenderer()
{
#ifdef D3D12
//	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->SetGraphicsRootDescriptorTable(0, ((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart());
#elif defined D3D11
	if (m_iType == VertexShader)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->VSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == GeometryShader)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->GSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == PixelShader)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->PSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == HullShader)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->HSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == DomainShader)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
#endif
}

void CBuffer::BindToRendererWithOffset(int parameterIndex, int offset)
{
	D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());
	renderer->m_pCommandList->SetGraphicsRootConstantBufferView(parameterIndex, CBV_UPLOAD_HEAP->GetGPUVirtualAddress() + m_iUploadHeapOffset);
}

void CBuffer::Update(size_t size)
{
#ifdef D3D12
	D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());
	/*
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	if (m_CPUHandle.ptr == 0)
	{
		// first time
		m_CPUHandle = renderer->m_CbvSrvUavHeapHandle;
		renderer->m_CbvSrvUavHeapHandle.Offset(1, renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}
	cbvDesc.BufferLocation = CBV_UPLOAD_HEAP->GetGPUVirtualAddress() + CBV_UPLOAD_HEAP_CURR_OFFSET;
	cbvDesc.SizeInBytes = (size + 255) & ~255;    // CB size is required to be 256-byte aligned.
	renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, m_CPUHandle);
	*/
	// offset upload heap pointer and descriptor pointer
	m_iUploadHeapOffset = CBV_UPLOAD_HEAP_CURR_OFFSET;
	CBV_UPLOAD_HEAP_CURR_OFFSET += (size + 255) & ~255;

	memcpy((void*)((uintptr_t)CBV_UPLOAD_HEAP_BEGIN + m_iUploadHeapOffset), m_Memory._data, size);

#elif defined D3D11
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->Map(m_Memory._buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	memcpy(mappedResources.pData, m_Memory._data, size);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->Unmap(m_Memory._buffer, 0);
#endif
}

};