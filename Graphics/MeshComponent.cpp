// MeshComponent.cpp

#include "D3D11Renderer.h"
#include "MeshComponent.h"
#include "MeshData.h"

MeshComponent::MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	m_pMeshData = new MeshData(pVertexData, iNumVerts, pIndexData, iNumIndices, dimension, renderType, typology, texture);
}

MeshComponent::MeshComponent(const char* filename)
{
	m_pMeshData = new MeshData(filename, eRenderType::STANDARD_MESH_WITH_SHADOW);
}

void MeshComponent::Draw()
{
	m_pMeshData->Render(SIMDMatrix4::Identity);
}