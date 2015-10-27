// MeshComponent.cpp

#include "D3D11Renderer.h"
#include "MeshComponent.h"
#include "MeshData.h"

MeshComponent::MeshComponent(const char* filename)
{
	m_pMeshData = new MeshData(filename, eRenderType::STANDARD_MESH_WITH_SHADOW);
}

void MeshComponent::Draw()
{
	//m_pMeshData->Render();
}