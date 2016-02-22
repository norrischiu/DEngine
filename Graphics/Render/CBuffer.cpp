#include "CBuffer.h"
#include "Math\simdmath.h"
#include "D3D11Renderer.h"

CBuffer::CBuffer(int type, size_t size)
	:m_iType(type)
{
	HRESULT hr;

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
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_Memory._buffer);
	assert(hr == S_OK);
}

void CBuffer::BindToRenderer()
{
	if (m_iType == VertexShader)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == GeometryShader)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->GSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
	if (m_iType == PixelShader)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	}
}

void CBuffer::Update(size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_Memory._buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	memcpy(mappedResources.pData, m_Memory._data, size);
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_Memory._buffer, 0);
}

