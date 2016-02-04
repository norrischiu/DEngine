// MeshComponent.cpp
#include "MeshComponent.h"
#include "D3D11Renderer.h"
#include "MeshData.h"
#include "Scene\SceneGraph.h"
#include "GameObject\GameObject.h"

MeshComponent::MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
{
	*m_pTransform = Matrix4::Identity;
	m_pMeshData = new MeshData(pVertexData, iNumVerts, pIndexData, iNumIndices, dimension, renderType, typology, texture);
}

MeshComponent::MeshComponent(const char* meshName)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
{
	*m_pTransform = Matrix4::Identity;
	m_pMeshData = new MeshData(meshName, eRenderType::STANDARD_MESH_WITH_BUMP);
	SceneGraph::GetInstance()->AddComponent(this);
}

void MeshComponent::SetOwner(GameObject * ptr)
{
	Component::SetOwner(ptr);
	m_pTransform = m_pOwner->GetTransform();
}


void MeshComponent::Draw()
{
	if (m_bVisible)
	{
		m_pMeshData->Render();
	}
}