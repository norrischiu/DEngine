// MeshComponent.cpp

#include "D3D11Renderer.h"
#include "MeshComponent.h"
#include "MeshData.h"
#include "Scene\SceneGraph.h"

MeshComponent::MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	Matrix4* temp = new Matrix4(Matrix4::Identity);
	m_pTransform = temp;
	m_pMeshData = new MeshData(pVertexData, iNumVerts, pIndexData, iNumIndices, dimension, renderType, typology, texture);
}

MeshComponent::MeshComponent(const char* filename)
{
	//m_pMeshData = new MeshData(filename, eRenderType::STANDARD_MESH_WITH_SHADOW);
	m_pMeshData = new MeshData(filename, eRenderType::STANDARD_MESH_WITH_BUMP);
	SceneGraph::GetInstance()->AddComponent(this);
}

void MeshComponent::Draw()
{
	m_pMeshData->Render(*m_pTransform);
}