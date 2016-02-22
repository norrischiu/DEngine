// MeshData.cpp

#define C_STR(string, text)\
		(string + text).c_str()

#include "MeshData.h"
#include "Math\simdmath.h"
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "Material.h"
#include "VertexFormat.h"
#include <float.h>
#pragma comment (lib, "D3DCompiler")

MeshData::MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const Vector3& dimension, const eMeshType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture)
	:m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_transformationMat(Matrix4::Identity),
	m_dimension(dimension)
{
	SetUpEnvironment(renderType, typology, texture);

	switch (renderType) {
	case eMeshType::V1P:
		m_iStride = sizeof(Vertex1P);
		break;
	case eMeshType::V1P1UV:
		m_iStride = sizeof(vertex1P1UV);
		break;
	}

	// Create vertex buffer
	m_pVertexBuffer = VertexBufferEngine::GetInstance()->CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride);
	// Create index buffer
	//m_iStartIndexLocation = IndexBufferEngine::GetCurrentIndex();
	m_pIndexBuffer = IndexBufferEngine::GetInstance()->CreateBufferFromRawData(pIndexData, iNumIndics);
	m_iNumIndics = iNumIndics;
}

MeshData::MeshData(void* pVertexData, const int iNumVerts, unsigned int * pIndexData, const int iNumIndics, unsigned int stride, bool streamOut)
{
	m_iStride = stride;
	m_iNumIndics = iNumIndics;
	// Create vertex buffer
	m_pVertexBuffer = VertexBufferEngine::GetInstance()->CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride, streamOut);
	m_bStreamOut = streamOut;
	// Create index buffer
	m_pIndexBuffer = IndexBufferEngine::GetInstance()->CreateBufferFromRawData(pIndexData, m_iNumIndics);
	m_Material.UseDefault();
}

MeshData::MeshData(const char* filename, int meshType)
{
	std::string sFileName(filename);

	VertexBufferEngine vertexEngine;
	IndexBufferEngine indexEngine;
	m_pIndexBuffer = (ID3D11Buffer*)indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);

	switch (meshType)
	{
		case eMeshType::OUTLINE:
			m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION, m_iStride);
			break;
		case eMeshType::STANDARD_MESH:
			m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE, m_iStride);
			break;
		case eMeshType::SKELETAL_MESH:
			m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(filename, eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS, m_iStride);
			break;
	}

	m_BoundingBox = AABB(vertexEngine.GetMinXYZ(), vertexEngine.GetMaxXYZ());
	m_Material.ReadFromFile(C_STR(sFileName, "_material.mate"), meshType);
}

MeshData::~MeshData() 
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();
}

void MeshData::SetUpEnvironment(const eMeshType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	m_renderType = renderType;			// Set render type
//	m_iTopology = typology; 			// Set primitive topology

	switch (renderType)
	{
	case eMeshType::V1P1UV:
		// Set vertex shader
		//m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
		// Set input layout
//		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P1UV.hlsl");
		// Set pixel shader
		//m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
		// Set texture resources view
//		m_pTexResourceView.emplace_back((ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(texture));
		// Set shader sampler state
		//m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
		//m_pBlendState = (ID3D11BlendState*)TextureManager::GetInstance()->GetBlendState();
		break;

	case eMeshType::V1P:
		// Set vertex shader
		//m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/VS_vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
		m_iStride = sizeof(Vertex1P);
		// Set input layout
//		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/VS_vertex1P.hlsl");
		// Set pixel shader
		//m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/PS_red.hlsl", D3D11_SHVER_PIXEL_SHADER);
		break;
	}

	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER2);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;
	//VS_CONSTANT_BUFFER2 vsConstData;
	//constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	//D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &m_pVSConstantBuffer);
	//D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);
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
	//D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	//VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	//pCBuffer->ProjectionMat = ProjectionMat;
	//pCBuffer->TransformationMat = ProjectionMat * m_transformationMat;
	//D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pVSConstantBuffer, 0);
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
	//D3D11Renderer::GetInstance()->m_pD3D11Context->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
//	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	//pCBuffer->ProjectionMat = ProjectionMat;
//	pCBuffer->TransformationMat = m_transformationMat;
	//D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(m_pVSConstantBuffer, 0);
}

void MeshData::Render()
{
	RenderTechnique* technique = m_Material.GetRenderTechnique();
	for (auto pass : technique->m_vRenderPasses)
	{
		pass->BindToRenderer();

		D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
		D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		if (m_bStreamOut)
		{
			D3D11Renderer::GetInstance()->m_pD3D11Context->DrawAuto();
		}
		else
		{
			D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
		}

		D3D11Renderer::GetInstance()->UnbindPSShaderResources(pass->GetTextureCount());
		D3D11Renderer::GetInstance()->UnbindRenderTargets();
	}
}

void MeshData::RenderUsingPass(RenderPass * pass)
{
	pass->BindToRenderer();

	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (m_bStreamOut)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->DrawAuto();
	}
	else
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
	}
}
