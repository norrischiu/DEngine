#include "SceneGraph.h"

SceneGraph* SceneGraph::m_pInstance;

void SceneGraph::Render()
{
	for (auto itr : m_tree)
	{
		itr->Draw();
	}
#ifdef _DEBUG
	for (auto itr : DEBUG_DRAWING_TREE)
	{
		itr->Draw();
	}
#endif
}

void SceneGraph::AddComponent(MeshComponent * meshComponent)
{
	m_tree.push_back(meshComponent);
}

void SceneGraph::ADD_DEBUG_DRAWING(MeshComponent * meshComponent)
{
	DEBUG_DRAWING_TREE.push_back(meshComponent);
}

