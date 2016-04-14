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

namespace DE
{

MeshData::MeshData(void* pVertexData, const int iNumVerts, unsigned int * pIndexData, const int iNumIndics, unsigned int stride, bool streamOut)
{
	VertexBufferEngine vertexEngine;
	IndexBufferEngine indexEngine;

	m_iStride = stride;
	m_iNumIndics = iNumIndics;
	// Create vertex buffer
	m_pVertexBuffer = vertexEngine.CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride, streamOut);
	m_bStreamOut = streamOut;
	// Create index buffer
	m_pIndexBuffer = indexEngine.CreateBufferFromRawData(pIndexData, m_iNumIndics);
	m_BoundingBox = AABB(vertexEngine.GetMinXYZ(), vertexEngine.GetMaxXYZ());
	m_Material.UseDefault();
}

MeshData::MeshData(const char* filename, int meshType)
{
	std::string sFileName(filename);
	sFileName = "../Assets/" + sFileName;

	VertexBufferEngine vertexEngine;
	IndexBufferEngine indexEngine;
	m_pIndexBuffer = (ID3D11Buffer*)indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);

	switch (meshType)
	{
	case eMeshType::OUTLINE:
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION, m_iStride);
		break;
	case eMeshType::STANDARD_MESH:
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE, m_iStride);
		break;
	case eMeshType::SKELETAL_MESH:
		m_pVertexBuffer = (ID3D11Buffer*)vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS, m_iStride);
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

};