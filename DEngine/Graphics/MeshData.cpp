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
#ifdef D3D12
	void* vertexBuffer = nullptr;
	void* indexBuffer = nullptr;

	vertexBuffer = vertexEngine.CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride, streamOut);
	m_bStreamOut = streamOut;
	indexBuffer = (ID3D11Buffer*) indexEngine.CreateBufferFromRawData(pIndexData, m_iNumIndics);

	m_VBV.BufferLocation = (UINT64)vertexBuffer;
	m_VBV.StrideInBytes = m_iStride;
	m_VBV.SizeInBytes = stride * iNumVerts;

	m_IBV.BufferLocation = (UINT64)indexBuffer;
	m_IBV.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	m_IBV.SizeInBytes = sizeof(UINT) * iNumIndics;
#elif defined D3D11
	m_pVertexBuffer = (ID3D11Buffer*) vertexEngine.CreateBufferFromRawData(pVertexData, iNumVerts, m_iStride, streamOut);
	m_bStreamOut = streamOut;
	m_pIndexBuffer = (ID3D11Buffer*) indexEngine.CreateBufferFromRawData(pIndexData, m_iNumIndics);
#endif
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

	indexBuffer = (ID3D11Buffer*)indexEngine.CreateBuffer(C_STR(sFileName, "_index.bufa"), m_iNumIndics);
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

#ifdef D3D12
	m_VBV.BufferLocation = (UINT64)vertexBuffer;
	m_VBV.StrideInBytes = m_iStride;
	m_VBV.SizeInBytes = bufferSize;

	m_IBV.BufferLocation = (UINT64)indexBuffer;
	m_IBV.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	m_IBV.SizeInBytes = m_iNumIndics * sizeof(UINT);
#elif defined D3D11
	m_pVertexBuffer = (ID3D11Buffer*)vertexBuffer;
	m_pIndexBuffer = (ID3D11Buffer*)indexBuffer;
#endif

	m_BoundingBox = AABB(vertexEngine.GetMinXYZ(), vertexEngine.GetMaxXYZ());
	m_Material.ReadFromFile(C_STR(sFileName, "_material.mate"), meshType);
}

MeshData::~MeshData()
{
#ifdef D3D11
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();
#endif
}

void MeshData::Render()
{
		m_Material.GetRenderPass()->BindToRenderer();
#ifdef D3D12
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetVertexBuffers(0, 1, &m_VBV);
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetIndexBuffer(&m_IBV);
#endif
#ifdef D3D11
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
#endif
		if (m_bStreamOut)
		{
			// TODO:
			//((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DrawAuto();
		}
		else
		{
#ifdef D3D12
			((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->DrawIndexedInstanced(m_iNumIndics, 1, 0, 0, 0);
#elif defined D3D11
			((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
#endif
		}

		//((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindPSShaderResources(technique->m_vRenderPasses[i]->GetTextureCount());
		//((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
}

void MeshData::RenderUsingPass(RenderPass * pass)
{
	pass->BindToRenderer();

#ifdef D3D12
#ifdef D3D12
	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetVertexBuffers(0, 1, &m_VBV);
	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->IASetIndexBuffer(&m_IBV);
#endif
	if (m_bStreamOut)
	{
		//((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DrawAuto();
	}
	else
	{
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->DrawIndexedInstanced(m_iNumIndics, 1, 0, 0, 0);
	}
#elif defined D3D11
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_iStride, &m_iVertexOffset);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (m_bStreamOut)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DrawAuto();
	}
	else
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
	}
#endif
}

};