#include "GBuffer.h"
#include "Light/LightManager.h"
#include "Light/PointLightComponent.h"
//#include "Light/DirectionalLight.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <DirectXMath.h>

struct VS_DEFERRED_CBUFFER
{
	Matrix4 WorldTransform;
	Matrix4 Transform;
};

struct PointLightGPU
{
	Vector4 vPos;
	Vector4 vAmbient;
	Vector4 vDiffuse;
	Vector4 vSpecular;
	float fIntensity;
};

struct PS_DEFERRED_CBUFFER
{
	PointLightGPU testLight;
	Matrix4 mProjectionInverse;
	//DirectionalLight testLight;
};

GBuffer::GBuffer()
{
	HRESULT hr;

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = LightManager::GetInstance()->GetPointLightVertices();
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex1P) * 8;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &m_pVertexBuffer);
	assert(hr == S_OK);

	// Set index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * 36;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Set index subresources data
	D3D11_SUBRESOURCE_DATA indexResourcesData;
	UINT pIndices[36] =
	{ 0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6 };
	indexResourcesData.pSysMem = pIndices;
	indexResourcesData.SysMemPitch = 0;
	indexResourcesData.SysMemSlicePitch = 0;

	// Create the index buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&indexBufferDesc, &indexResourcesData, &m_pIndexBuffer);
	assert(hr == S_OK);

	D3D11_BUFFER_DESC psconstBufferDesc;
	psconstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	psconstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	psconstBufferDesc.ByteWidth = sizeof(PS_DEFERRED_CBUFFER);
	psconstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	psconstBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA psconstResourcesData;
	PS_DEFERRED_CBUFFER temp;
	psconstResourcesData.pSysMem = &temp;
	psconstResourcesData.SysMemPitch = 0;
	psconstResourcesData.SysMemSlicePitch = 0;
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&psconstBufferDesc, &psconstResourcesData, &m_pPSConstantBuffer);

	// Set vertex buffer
	/*
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
	*/
	m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_gbuffer.hlsl", D3D11_SHVER_VERTEX_SHADER);
	m_pLightVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
	m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_gbuffer.hlsl");
	m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_phong_gbuffer.hlsl", D3D11_SHVER_PIXEL_SHADER);
	m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::NONE);

	// Create the constant buffer
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_DEFERRED_CBUFFER);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA constResourcesData;
	VS_DEFERRED_CBUFFER vsConstData;
	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_pVSConstantBuffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	D3D11Renderer::GetInstance()->m_pD3D11Device->CreateDepthStencilState(&dsDesc, &m_pDSState);
}

void GBuffer::LightStencilCheck()
{	
	unsigned int m_iVertexOffset = 0;
	unsigned int m_iStride = sizeof(Vertex1P);
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pLightVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(nullptr, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState(D3D11Renderer::GetInstance()->m_pCullNoneRSState);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetDepthStencilState(m_pDSState, 1);

	// Set stencil buffer
	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		// Update VS cbuffer
		D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
		VS_DEFERRED_CBUFFER* pVSCBuffer = (VS_DEFERRED_CBUFFER*)mappedResources.pData;
		Matrix4 scale;
		Matrix4 trans;
		trans.CreateTranslation(LightManager::GetInstance()->GetLightAt(i)->GetPosition());
		scale.CreateScale(LightManager::GetInstance()->GetLightAt(i)->GetRadius());
		pVSCBuffer->Transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * trans * scale;
		D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pVSConstantBuffer, 0);

		// Binding
		D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);
		D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(36, 0, 0);
	}

	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void GBuffer::Render()
{
	LightStencilCheck();

	unsigned int m_iVertexOffset = 0;
	unsigned int m_iStride = sizeof(Vertex1P);
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	//D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState(D3D11Renderer::GetInstance()->m_pCullBackRSState);
	D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState(D3D11Renderer::GetInstance()->m_pCullNoneRSState);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetDepthStencilState(D3D11Renderer::GetInstance()->m_pOffDepthStencilState, 1);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, RT_NUM, D3D11Renderer::GetInstance()->m_pShaderResourceView);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetBlendState(D3D11Renderer::GetInstance()->m_pBlendState, NULL, 0xFFFFFFFF);
	
	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		// Update VS cbuffer
		D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
		VS_DEFERRED_CBUFFER* pVSCBuffer = (VS_DEFERRED_CBUFFER*)mappedResources.pData;
		Matrix4 scale;
		Matrix4 trans;
		trans.CreateTranslation(LightManager::GetInstance()->GetLightAt(i)->GetPosition());
		scale.CreateScale(LightManager::GetInstance()->GetLightAt(i)->GetRadius());
		pVSCBuffer->Transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * trans * scale;
		pVSCBuffer->WorldTransform =  D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * trans * scale;
		D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pVSConstantBuffer, 0);

		// Update PS cbuffer
		D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pPSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
		PS_DEFERRED_CBUFFER* pPSCBuffer = (PS_DEFERRED_CBUFFER*)mappedResources.pData;
		PointLightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);
		pPSCBuffer->testLight.vDiffuse = currLight->GetDiffuse();
		pPSCBuffer->testLight.vAmbient = currLight->GetAmbient();
		pPSCBuffer->testLight.fIntensity = currLight->GetIntensity();
		Vector3 pos = currLight->GetPosition();
		pos.Transform(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());
		pPSCBuffer->testLight.vPos = pos;
		pPSCBuffer->mProjectionInverse = D3D11Renderer::GetInstance()->GetCamera()->GetPerspectiveMatrix().Inverse();
		D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pPSConstantBuffer, 0);

		// Binding
		D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(0, 1, &m_pPSConstantBuffer);
		D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetRenderTargets(1, &D3D11Renderer::GetInstance()->m_pBackBufferRTView, D3D11Renderer::GetInstance()->m_pDepthStencilReadOnlyView);
		D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(36, 0, 0);
	}

	// Unbound the resources
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr, nullptr};
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, 3, null);
}

