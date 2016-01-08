#include "SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Graphics/D3D11Renderer.h"

SceneGraph* SceneGraph::m_pInstance;

void SceneGraph::Render()
{
	for (auto itr : m_tree)
	{
		itr->Draw();
	}
}

void SceneGraph::RENDER_DEBUG_DRAWING()
{
#ifdef _DEBUG
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetDepthStencilState(D3D11Renderer::GetInstance()->m_pOffDepthOffStencilState, 1);
	D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState(D3D11Renderer::GetInstance()->m_pFillWireRSState);
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

