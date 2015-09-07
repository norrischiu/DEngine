// MeshData.cpp

#include "MeshData.h"
#include "..\Math\simdmath.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "VertexFormat.h"

#pragma comment (lib, "D3DCompiler")

typedef SIMDVector3 Vector3;

struct VS_CONSTANT_BUFFER
{
	Matrix4 CameraMat;
	Matrix4 ProjectionMat;
};

MeshData::MeshData(const char* filename, int renderType)
	: m_camera(Vector3(0, 3, 10), Vector3(1, 0, 0), Vector3(0, 1, 0))
	//, m_cFilename(filename)
	, m_pIndexBuffer(nullptr)
{
	HRESULT hr;
	
	switch (renderType)
	{
		case eRenderType::OUTLINE:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::getInstance()->GetShader("Shaders/vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::getInstance()->GetInputLayout("Shaders/vertex1P.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::getInstance()->GetShader("Shaders/red.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

			break;
		case eRenderType::STANDARD_MESH:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::getInstance()->GetShader("Shaders/vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::getInstance()->GetInputLayout("Shaders/vertex1P1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::getInstance()->GetShader("Shaders/texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView = (ID3D11ShaderResourceView*)TextureManager::getInstance()->GetTexture("dragon_texture.dds");
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::getInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

			break;
		case eRenderType::STANDARD_MESH_WITH_SHADOW:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::getInstance()->GetShader("Shaders/vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::getInstance()->GetInputLayout("Shaders/vertex1P1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::getInstance()->GetShader("Shaders/texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView = (ID3D11ShaderResourceView*)TextureManager::getInstance()->GetTexture("dragon_texture.dds");
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::getInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			// Set primitive topology
			m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

			break;
	}

	// Create vertex buffer
	VertexBufferEngine vertexEngine;
	vertexEngine.CreateBuffer("dragon_vertex.bufa", eVertexFormat::POSITION_TEXTURE);
	// Create index buffer
	IndexBufferEngine indexEngine;
	m_iStartIndexLocation = IndexBufferEngine::g_iCurrIndex;
	indexEngine.CreateBuffer("dragon_index.bufa");
	m_iNumIndics = IndexBufferEngine::g_iCurrIndex - m_iStartIndexLocation;
	
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
	vsConstData.CameraMat.CreateLookAt(Vector3(0.0f, 3.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); // temp
	vsConstData.ProjectionMat.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	vsConstData.ProjectionMat.Multiply(vsConstData.CameraMat);
	Matrix4 rotation;
	rotation.CreateRotationY(0.785398163f); // 45 degree
	vsConstData.ProjectionMat.Multiply(rotation);

	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &g_pConstantBuffer);
	assert(hr == S_OK);
	D3D11Renderer::getInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	
}

void MeshData::Update()
{
	// Set constant buffer description
	m_camera.update();
	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	ProjectionMat.Multiply(m_camera.getViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::getInstance()->m_pD3D11Context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*) mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	D3D11Renderer::getInstance()->m_pD3D11Context->Unmap(g_pConstantBuffer, 0);
}

void MeshData::Render()
{
	//Update();
	
	// Binding
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::getInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::getInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);

	if (m_pTexResourceView)
	{
		D3D11Renderer::getInstance()->m_pD3D11Context->PSSetShaderResources(0, 1, &m_pTexResourceView);
		D3D11Renderer::getInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
	}

	D3D11Renderer::getInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, m_iStartIndexLocation, 0);
}