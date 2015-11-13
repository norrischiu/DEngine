#include "GBuffer.h"
#include "../Light/PointLight.h"
#include "../Light/DirectionalLight.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <DirectXMath.h>

struct VS_DEFERRED_CBUFFER
{
	Matrix4 Transform;
};

struct PS_DEFERRED_CBUFFER
{
	//PointLight testLight;
	DirectionalLight testLight;
};

GBuffer::GBuffer()
{
	HRESULT hr;

	//PointLight testLight(Vector3(0, 0, 10), 50, Vector4(0.01, 0.01, 0.01), Vector4(1, 1, 1), Vector4(1, 1, 1), 50);
	DirectionalLight testLight(Vector3(0, 0, 3, 0), Vector4(0.1, 0.1, 0.1), Vector4(0.5, 0.5, 0.5), Vector4(1, 1, 1), 5);

	PS_DEFERRED_CBUFFER psConstData;
	psConstData.testLight = testLight;

	D3D11_BUFFER_DESC psconstBufferDesc;
	psconstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	psconstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	psconstBufferDesc.ByteWidth = sizeof(PS_DEFERRED_CBUFFER);
	psconstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	psconstBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA psconstResourcesData;
	psconstResourcesData.pSysMem = &psConstData; psconstResourcesData.SysMemPitch = 0; psconstResourcesData.SysMemSlicePitch = 0;
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&psconstBufferDesc, &psconstResourcesData, &m_pPSConstantBuffer);

	// Set vertex buffer
	vertex1P1UV* pVertexData = new vertex1P1UV[6];
	float left = (1024 / 2) * -1;
	float right = left + 1024;
	float top = 768 / 2;
	float bottom = top - 768;
	pVertexData[0].m_pos = Vector3(left, top, 0);
	pVertexData[0].m_UV[0] = 0;
	pVertexData[0].m_UV[1] = 0;
	pVertexData[1].m_pos = Vector3(right, top, 0);
	pVertexData[1].m_UV[0] = 1;
	pVertexData[1].m_UV[1] = 0;
	pVertexData[2].m_pos = Vector3(left, bottom, 0);
	pVertexData[2].m_UV[0] = 0;
	pVertexData[2].m_UV[1] = 1;
	pVertexData[3].m_pos = Vector3(left, bottom, 0);
	pVertexData[3].m_UV[0] = 0;
	pVertexData[3].m_UV[1] = 1;
	pVertexData[4].m_pos = Vector3(right, top, 0);
	pVertexData[4].m_UV[0] = 1;
	pVertexData[4].m_UV[1] = 0;
	pVertexData[5].m_pos = Vector3(right, bottom, 0);
	pVertexData[5].m_UV[0] = 1;
	pVertexData[5].m_UV[1] = 1;

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = pVertexData;
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertex1P1UV) * 6;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &m_pVertexBuffer);
	assert(hr == S_OK);

	// Set index buffer
	UINT pIndexData[6];
	for (int i = 0; i < 6; i++)
	{
		pIndexData[i] = i;
	}
	// Set index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * 6;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Set index subresources data
	D3D11_SUBRESOURCE_DATA indexResourcesData;
	indexResourcesData.pSysMem = pIndexData;
	indexResourcesData.SysMemPitch = 0;
	indexResourcesData.SysMemSlicePitch = 0;

	// Create the index buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&indexBufferDesc, &indexResourcesData, &m_pIndexBuffer);
	assert(hr == S_OK);

	// Set vertex shader
	m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_gbuffer.hlsl", D3D11_SHVER_VERTEX_SHADER);
	// Set input layout
	m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_gbuffer.hlsl");
	// Set pixel shader
	m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_phong_gbuffer.hlsl", D3D11_SHVER_PIXEL_SHADER);
	// Set shader sampler state
	m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);

	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_DEFERRED_CBUFFER);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;
	VS_DEFERRED_CBUFFER vsConstData;
	Matrix4 OrthoMat;
	OrthoMat.CreateOrthographicProj(1024, 768, 0.01f, 1000.0f);
	vsConstData.Transform = OrthoMat;
	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_pConstantBuffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void GBuffer::Render()
{
	// Binding
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	unsigned int m_iVertexOffset = 0;
	unsigned int m_iStride = sizeof(vertex1P1UV);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, RT_NUM, D3D11Renderer::GetInstance()->m_pShaderResourceView);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(0, 1, &m_pPSConstantBuffer);

	D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(6, 0, 0);

	// Unbound the shader resources
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr };
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, 2, null);
}
