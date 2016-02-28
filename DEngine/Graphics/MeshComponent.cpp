// MeshComponent.cpp
#include "MeshComponent.h"
#include "D3D11Renderer.h"
#include "MeshData.h"
#include "Scene\SceneGraph.h"
#include "GameObject\GameObject.h"

MeshComponent::MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eMeshType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
{
	*m_pTransform = Matrix4::Identity;
	m_pMeshData = new MeshData(pVertexData, iNumVerts, pIndexData, iNumIndices, dimension, renderType, typology, texture);
}

MeshComponent::MeshComponent(MeshData* meshData)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
	, m_pMeshData(meshData)
{
	*m_pTransform = Matrix4::Identity;
	//SceneGraph::GetInstance()->AddComponent(this);
}

MeshComponent::MeshComponent(const char* meshName, int type)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
{
	*m_pTransform = Matrix4::Identity;
	m_pMeshData = new MeshData(meshName, type);
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