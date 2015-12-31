// MeshData.cpp

#define C_STR(string, text)\
		(string + text).c_str()

#include "MeshData.h"
#include "Math\simdmath.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "VertexFormat.h"
#include "Light/PointLight.h"
#include "Material.h"

#include <float.h>
#pragma comment (lib, "D3DCompiler")

struct VS_CONSTANT_BUFFER
{
	Matrix4 CameraMat;
	Matrix4 ProjectionMat;
	Matrix4 TransformationMat;
};

struct VS_CONSTANT_BUFFER2
{
	Matrix4 WorldTransform;
	Matrix4 Transform;
};

MeshData::MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const Vector3& dimension, const eRenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture)
	:m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_transformationMat(Matrix4::Identity),
	m_dimension(dimension)
{
	SetUpEnvironment(renderType, typology, texture);

	switch (renderType) {
	case eRenderType::V1P:
		m_iStride = sizeof(Vertex1P);
		break;
	case eRenderType::V1P1UV:
		m_iStride = sizeof(vertex1P1UV);
		break;
	}

	// Create vertex buffer
	m_pVertexBuffer = VertexBufferEngine::GetInstance()->CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride);
	// Create index buffer
	m_iStartIndexLocation = IndexBufferEngine::GetCurrentIndex();
	m_pIndexBuffer = IndexBufferEngine::GetInstance()->CreateBufferFromRawData(pIndexData, iNumIndics);
	m_iNumIndics = iNumIndics;
}


MeshData::MeshData(const char* filename, int renderType, const char* VSFilename, const char* PSFilename)
	: m_pIndexBuffer(nullptr)
	, m_iTexResourceNum(0)
	, m_pTexResourceView()
{
	HRESULT hr;
	std::string sFileName(filename);

	VertexBufferEngine vertexEngine;
	IndexBufferEngine indexEngine;
	m_pIndexBuffer = (ID3D11Buffer*)indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);

	switch (renderType)
	{
	case eRenderType::OUTLINE:
		// Set vertex shader
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
		// Set input layout
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P.hlsl");
		// Set pixel shader
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_red.hlsl", D3D11_SHVER_PIXEL_SHADER);
		// Set primitive topology
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		// Create vertex buffer
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION, m_iStride);

		break;
	case eRenderType::STANDARD_MESH:
		// Set vertex shader
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
		// Set input layout
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P1UV.hlsl");
		// Set pixel shader
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
		// Set texture resources view
		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_texture.dds")));
		m_iTexResourceNum = 1;
		// Set shader sampler state
		m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
		// Set primitive topology
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		// Create vertex buffer
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_TEXTURE, m_iStride);

		break;
	case eRenderType::STANDARD_MESH_WITH_SHADOW:
		// Set vertex shader
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P1N1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
		// Set input layout
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P1N1UV.hlsl");
		// Set pixel shader
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_vertex1P1N1UV_deferred.hlsl", D3D11_SHVER_PIXEL_SHADER);
		// Set texture resources view
		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_texture.dds")));
		m_iTexResourceNum = 1;
		// Set shader sampler state
		m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
		// Set primitive topology
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		// Create vertex buffer
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_NORMAL_TEXTURE, m_iStride);

		break;
	case eRenderType::STANDARD_MESH_WITH_BUMP:
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P1N1T1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P1N1T1UV.hlsl");
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_vertex1P1N1T1UV_deferred.hlsl", D3D11_SHVER_PIXEL_SHADER);
		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_texture.dds")));
		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(C_STR(sFileName, "_normal.dds")));
		m_iTexResourceNum = 2;
		m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE, m_iStride);
		break;
		// RenderPass standardMeshWithBump;
		// standardMeshWithBump.Bind()
		// standardMeshWithBump.Unbind()
		// standardMeshWithBump.UpdateConstantBuffer()

	case eRenderType::CUSTOM_SHADERS:
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader(VSFilename, D3D11_SHVER_VERTEX_SHADER);
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout(VSFilename);
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader(PSFilename, D3D11_SHVER_PIXEL_SHADER);
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}

	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER2);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;
	VS_CONSTANT_BUFFER2 vsConstData;
	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_pConstantBuffer);
	assert(hr == S_OK);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

MeshData::~MeshData() {
	if (m_pVS)
		m_pVS->Release();
	if (m_pPS)
		m_pPS->Release();
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();
	if (m_pConstantBuffer)
		m_pConstantBuffer->Release();
	if (m_pInputLayout)
		m_pInputLayout->Release();
	for (auto itr : m_pTexResourceView)
	{
		if (itr)
			itr->Release();
	}
	if (m_pSamplerState)
		m_pSamplerState->Release();
	if (m_pBlendState)
		m_pBlendState->Release();
}

void MeshData::SetUpEnvironment(const eRenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	m_renderType = renderType;			// Set render type
	m_iTopology = typology; 			// Set primitive topology

	switch (renderType)
	{
	case eRenderType::V1P1UV:
		// Set vertex shader
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
		// Set input layout
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P1UV.hlsl");
		// Set pixel shader
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
		// Set texture resources view
		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(texture));
		// Set shader sampler state
		m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
		m_pBlendState = (ID3D11BlendState*)TextureManager::GetInstance()->GetBlendState();
		break;

	case eRenderType::V1P:
		// Set vertex shader
		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
		m_iStride = sizeof(Vertex1P);
		// Set input layout
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P.hlsl");
		// Set pixel shader
		m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_red.hlsl", D3D11_SHVER_PIXEL_SHADER);
		break;
	}

	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER2);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;
	VS_CONSTANT_BUFFER2 vsConstData;
	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_pConstantBuffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void MeshData::Transform(const float scalar, const Vector3 rotation, const Vector3 translation) {
	const float EPS = std::numeric_limits<float>::epsilon();
	Matrix4 Transform;

	if (fabs(scalar - 1.0f) > EPS) {
		Transform.CreateScale(scalar);
		m_transformationMat.Multiply(Transform);
	}
	if ((fabs(rotation.GetX()) > EPS) || +(fabs(rotation.GetY()) > EPS) || (fabs(rotation.GetZ()) > EPS)) {
		bool is_rotation_about_origin = fabs(rotation.GetW() - 1.0f) < 0.005f;
		Vector3 origin(0.0f, 0.0f, 0.0f);
		origin.TransformAsVector(m_transformationMat);

		if (is_rotation_about_origin) {
			Vector3 translationOrigin(-origin.GetX(), -origin.GetY(), -origin.GetZ());
			Transform.CreateTranslation(translationOrigin);
			m_transformationMat.Multiply(Transform);
		}

		if (rotation.GetX() > EPS) {
			Transform.CreateRotationX(rotation.GetX());
			m_transformationMat.Multiply(Transform);
		}

		if (rotation.GetY() > EPS) {
			Transform.CreateRotationY(rotation.GetY());
			m_transformationMat.Multiply(Transform);
		}

		if (rotation.GetZ() > EPS) {
			Transform.CreateRotationZ(rotation.GetZ());
			m_transformationMat.Multiply(Transform);
		}

		if (is_rotation_about_origin) {
			Transform.CreateTranslation(origin);
			m_transformationMat.Multiply(Transform);
		}
	}

	if ((fabs(translation.GetX()) > EPS) || (fabs(translation.GetY()) > EPS) + (fabs(translation.GetZ()) > EPS)) {
		Transform.CreateTranslation(translation);
		m_transformationMat.Multiply(Transform);
	}

	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(
		0.785398163f,
		1024.0f / 768.0f,
		1.0f,
		100.0f
		);
	ProjectionMat.Multiply(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	pCBuffer->TransformationMat = ProjectionMat * m_transformationMat;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pConstantBuffer, 0);
}

void MeshData::Update()
{
	// Set constant buffer description
	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(
		0.785398163f,
		1024.0f / 768.0f,
		1.0f,
		100.0f
		);
	ProjectionMat.Multiply(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	pCBuffer->TransformationMat = m_transformationMat;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pConstantBuffer, 0);
}

void MeshData::Update(Matrix4 transform)
{
	// Set constant buffer description
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER2* pCBuffer = (VS_CONSTANT_BUFFER2*)mappedResources.pData;
	pCBuffer->WorldTransform = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * transform;
	transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * transform;
	pCBuffer->Transform = transform;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pConstantBuffer, 0);
}

void MeshData::Render(Matrix4 transform)
{
	Update(transform);

	// Binding
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetBlendState(nullptr, NULL, 0xFFFFFFFF);
	if (!m_pTexResourceView.empty())
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, m_pTexResourceView.size(), m_pTexResourceView.data());
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
	}

	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
}