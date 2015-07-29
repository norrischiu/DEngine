// MeshInstance.cpp

#include "D3D11Renderer.h"
#include "MeshInstance.h"
#include "MeshData.h"

MeshInstance::MeshInstance(const char* filename)
{
	m_pMeshData = new MeshData(NULL);
}

void MeshInstance::Draw()
{
	m_pMeshData->Render();
}