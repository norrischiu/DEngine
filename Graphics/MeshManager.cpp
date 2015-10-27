// MeshManager.cpp

#include "MeshManager.h"

MeshManager* MeshManager::m_pInstance;

void MeshManager::Destruct()
{
	m_MeshDataMap.clear();
}