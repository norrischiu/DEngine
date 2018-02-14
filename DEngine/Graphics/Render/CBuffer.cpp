#include "CBuffer.h"
#include "Math\simdmath.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

CBuffer::CBuffer(int type, size_t size)
: m_iType(type)
, m_iSize(size)
{
	HRESULT hr;

#ifdef D3D12
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
	renderer->m_pCommandList->SetGraphicsRootConstantBufferView(parameterIndex, reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS>(m_Memory._gpuHandle._gpuAddress));
}

void CBuffer::Update(size_t size)
{
#ifdef D3D12
	D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());

	m_Memory._gpuHandle = renderer->m_pConstantBufferAllocator->Allocate(size);
	memcpy(m_Memory._gpuHandle._cpuAddress, m_Memory._data, size);

#elif defined D3D11
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->Map(m_Memory._buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	memcpy(mappedResources.pData, m_Memory._data, size);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->Unmap(m_Memory._buffer, 0);
#endif
}

};