// MeshInstance.cpp

#include "D3D11Renderer.h"
#include "MeshInstance.h"
#include "MeshData.h"

MeshInstance::MeshInstance(const char* filename)
{
	m_pMeshData = new MeshData("dragon", eRenderType::STANDARD_MESH);
}

void MeshInstance::Draw()
{
	m_pMeshData->Render();
}