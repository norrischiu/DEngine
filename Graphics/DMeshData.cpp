// DMeshData.cpp

#define C_STR(string, text)\
		(string + text).c_str()

#include "DMeshData.h"
#include "..\Math\simdmath.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "VertexFormat.h"
#include "../Light/PointLight.h"
#include "DeferredRenderer.h"

#pragma comment (lib, "D3DCompiler")

typedef SIMDVector3 Vector3;

struct VS_CONSTANT_BUFFER
{
	Matrix4 WorldTransform;
	Matrix4 Transform;
};

struct PS_CONSTANT_BUFFER
{
	PointLight testLight;
};

DMeshData::DMeshData(const char* filename, int renderType)
	: m_pIndexBuffer(nullptr)
	, m_iTexResourceNum(0)
	, m_pTexResourceView()
{
	HRESULT hr;
	std::string sFileName(filename);

	PointLight testLight(Vector3(0,0,0), 50, Vector4(0.1,0.1,0.1), Vector4(1,1,1), Vector4(1,1,1), 50);

	PS_CONSTANT_BUFFER psConstData;
	psConstData.testLight = testLight;
	ID3D11Buffer* gbuff;

	D3D11_BUFFER_DESC psconstBufferDesc;
	psconstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	psconstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	psconstBufferDesc.ByteWidth = sizeof(PS_CONSTANT_BUFFER);
	psconstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	psconstBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA psconstResourcesData;
	psconstResourcesData.pSysMem = &psConstData; psconstResourcesData.SysMemPitch = 0; psconstResourcesData.SysMemSlicePitch = 0;
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&psconstBufferDesc, &psconstResourcesData, &gbuff);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(0, 1, &gbuff);


	VertexBufferEngine vertexEngine;
	m_iVertexOffset = 0; // temp
	IndexBufferEngine indexEngine;
	m_pIndexBuffer = (ID3D11Buffer*) indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);

	switch (renderType)
	{
		case eRenderType::OUTLINE:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/red.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			// Create vertex buffer
			m_pVertexBuffer = (ID3D11Buffer*) vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION, m_iStride);

			break;
		case eRenderType::STANDARD_MESH:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_texture.dds")));
			m_iTexResourceNum = 1;
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			// Create vertex buffer
			m_pVertexBuffer = (ID3D11Buffer*) vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_TEXTURE, m_iStride);

			break;
		case eRenderType::STANDARD_MESH_WITH_SHADOW:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P1N1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P1N1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/deferred.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_texture.dds")));
			m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_normal.dds")));
			m_iTexResourceNum = 2;
			//m_pTexResourceView.emplace_back(DeferredRenderer::GetInstance()->m_pShaderResourceView[0]);
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			// Create vertex buffer
			m_pVertexBuffer = (ID3D11Buffer*) vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_NORMAL_TEXTURE, m_iStride);

			break;
	}
	
	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;

	VS_CONSTANT_BUFFER vsConstData;
	vsConstData.Transform.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	vsConstData.Transform.Multiply(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());

	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &g_pConstantBuffer);
	assert(hr == S_OK);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
}

void DMeshData::Update(Matrix4 transform)
{
	// Set constant buffer description
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->WorldTransform = transform;
	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	transform = ProjectionMat * D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * transform; // temp
	pCBuffer->Transform = transform;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(g_pConstantBuffer, 0);
}

void DMeshData::Render(Matrix4 transform)
{
	Update(transform);
	
	// Binding
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (!m_pTexResourceView.empty())
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, m_iTexResourceNum, &m_pTexResourceView[0]);
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
	}

	D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
}