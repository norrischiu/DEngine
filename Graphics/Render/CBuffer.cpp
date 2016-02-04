#include "CBuffer.h"
#include "Math\simdmath.h"
#include "D3D11Renderer.h"

CBuffer::CBuffer(int type, size_t sizeVS, size_t sizePS)
	:m_type(type)
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
	if (m_type == VS_ONLY || m_type == VS_PS)
	{
		constBufferDesc.ByteWidth = sizeVS;
		VS.m_data = malloc(sizeVS);
		constResourcesData.pSysMem = VS.m_data;
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &VS.m_pConstantBuffer);
		assert(hr == S_OK);
	}
	if (m_type == PS_ONLY || m_type == VS_PS)
	{
		constBufferDesc.ByteWidth = sizePS;
		PS.m_data = malloc(sizePS);
		constResourcesData.pSysMem = PS.m_data;
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &PS.m_pConstantBuffer);
		assert(hr == S_OK);
	}
}

void CBuffer::BindToRenderer()
{
	if (m_type == VS_ONLY || m_type == VS_PS)
		D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(m_iSlotID, 1, &VS.m_pConstantBuffer);
	if (m_type == PS_ONLY || m_type == VS_PS)
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(m_iSlotID, 1, &PS.m_pConstantBuffer);
}

void CBuffer::Update(size_t sizeVS, size_t sizePS)
{
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	if (sizeVS != NULL)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->Map(VS.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
		memcpy(mappedResources.pData, VS.m_data, sizeVS);
		D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(VS.m_pConstantBuffer, 0);
	}
	if (sizePS != NULL)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->Map(PS.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
		memcpy(mappedResources.pData, PS.m_data, sizePS);
		D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(PS.m_pConstantBuffer, 0);
	}

}

