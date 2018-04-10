// MeshData.cpp
#define C_STR(string, text)\
		(string + text).c_str()

#include "MeshData.h"
#include "Math\simdmath.h"
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "Material.h"
#include "VertexFormat.h"
#include "d3dx12.h"
#include "D3D12Renderer.h"
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
	// Create vertex buffer and index buffer
	void* vertexBuffer = nullptr;
	void* indexBuffer = nullptr;

	vertexBuffer = vertexEngine.CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride, streamOut);
	m_bStreamOut = streamOut;
	indexBuffer = indexEngine.CreateBufferFromRawData(pIndexData, m_iNumIndics);

	m_VBV.BufferLocation = (UINT64)vertexBuffer;
	m_VBV.StrideInBytes = m_iStride;
	m_VBV.SizeInBytes = stride * iNumVerts;

	m_IBV.BufferLocation = (UINT64)indexBuffer;
	m_IBV.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	m_IBV.SizeInBytes = sizeof(UINT) * iNumIndics;

	m_BoundingBox = AABB(vertexEngine.GetMinXYZ(), vertexEngine.GetMaxXYZ());
	m_Material.UseDefault();
}

MeshData::MeshData(const char* filename, int meshType)
{
	std::string sFileName(filename);
	sFileName = "../Assets/" + sFileName;

	VertexBufferEngine vertexEngine;
	IndexBufferEngine indexEngine;
	void* vertexBuffer = nullptr;
	void* indexBuffer = nullptr;
	unsigned int bufferSize;

	indexBuffer = indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);
	switch (meshType)
	{
	case eMeshType::OUTLINE:
		vertexBuffer = vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION, m_iStride, bufferSize);
		break;
	case eMeshType::STANDARD_MESH:
		vertexBuffer = vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE, m_iStride, bufferSize);
		break;
	case eMeshType::SKELETAL_MESH:
		vertexBuffer = vertexEngine.CreateBuffer(sFileName.c_str(), eVertexFormat::POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS, m_iStride, bufferSize);
		break;
	}

	m_VBV.BufferLocation = (UINT64)vertexBuffer;
	m_VBV.StrideInBytes = m_iStride;
	m_VBV.SizeInBytes = bufferSize;

	m_IBV.BufferLocation = (UINT64)indexBuffer;
	m_IBV.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	m_IBV.SizeInBytes = m_iNumIndics * sizeof(UINT);

	m_BoundingBox = AABB(vertexEngine.GetMinXYZ(), vertexEngine.GetMaxXYZ());
	m_Material.ReadFromFile(C_STR(sFileName, "_material.mate"), meshType);
}

MeshData::~MeshData()
{
}

void MeshData::Render(Renderer* renderer)
{
		m_Material.GetRenderPass()->BindToRenderer(renderer);
		renderer->m_pCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_VBV);
		renderer->m_pCommandList->IASetIndexBuffer(&m_IBV);
		if (m_bStreamOut)
		{
			// TODO:
		}
		else
		{
			renderer->m_pCommandList->DrawIndexedInstanced(m_iNumIndics, 1, 0, 0, 0);
		}
}

void MeshData::RenderUsingPass(Renderer* renderer, RenderPass * pass)
{
	pass->BindToRenderer(renderer);

	Renderer::GetInstance()->m_pCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Renderer::GetInstance()->m_pCommandList->IASetVertexBuffers(0, 1, &m_VBV);
	Renderer::GetInstance()->m_pCommandList->IASetIndexBuffer(&m_IBV);

	if (m_bStreamOut)
	{
	}
	else
	{
		Renderer::GetInstance()->m_pCommandList->DrawIndexedInstanced(m_iNumIndics, 1, 0, 0, 0);
	}
}

};