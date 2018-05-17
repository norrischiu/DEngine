// MeshComponent.cpp
#include "MeshComponent.h"
#include "MeshData.h"
#include "Scene\SceneGraph.h"
#include "GameObject\GameObject.h"

namespace DE
{

MeshComponent::MeshComponent(MeshData* meshData)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
	, m_pMeshData(meshData)
{
	m_ID = ComponentID;

	*m_pTransform = Matrix4::Identity;
	// SceneGraph::GetInstance()->AddComponent(this);
}

MeshComponent::MeshComponent(const char* meshName, int type)
	: Component()
	, m_pTransform(new Matrix4())
	, m_bVisible(true)
{
	m_ID = ComponentID;

	*m_pTransform = Matrix4::Identity;
	m_pMeshData = new MeshData(meshName, type);
	m_Material.ReadFromFile(meshName);
	SceneGraph::GetInstance()->AddComponent(this);
}

MeshData* MeshComponent::GetMeshData()
{
	return m_pMeshData;
}

void MeshComponent::SetOwner(GameObject * ptr)
{
	Component::SetOwner(ptr);
	m_pTransform = m_pOwner->GetTransform();
}


Material * MeshComponent::GetMaterial()
{
	return &m_Material;
}

bool MeshComponent::IsVisible()
{
	return m_bVisible;
}

void MeshComponent::SetVisibility(bool visible)
{
	m_bVisible = true;
}

Matrix4 * MeshComponent::GetTransform()
{
	return m_pTransform;
}

void MeshComponent::Draw(Renderer* renderer)
{
	if (m_bVisible)
	{
		m_Material.BindToRenderer(renderer);
		m_pMeshData->Render(renderer);
	}
}

};
