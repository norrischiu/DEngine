#include "SceneGraph.h"

SceneGraph* SceneGraph::m_pInstance;

void SceneGraph::Render()
{
	for (auto itr : m_tree)
	{
		itr->Draw();
	}
}

void SceneGraph::AddComponent(MeshComponent * meshComponent)
{
	m_tree.push_back(meshComponent);
}
