#include "SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Object\CameraComponent.h"
#include "Light\LightManager.h"
#include "GameObject\GameObject.h"
#include "Graphics\Render\State.h"
#include "Graphics\D3D11Renderer.h"
#include "Render\VSPerObjectCBuffer.h"
#include "Render\PSPerMaterialCBuffer.h"

SceneGraph* SceneGraph::m_pInstance;

SceneGraph::SceneGraph()
{
	m_pVSCBuffer = new VSPerObjectCBuffer;
	m_pPSCBuffer = new PSPerMaterialCBuffer;
}

void SceneGraph::FrustumCulling(Frustum frustum)
{
	for (auto itr : m_tree)
	{
		AABB meshBound = itr->m_pMeshData->GetBoundingBox();
		Matrix4 cameraSpace = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *itr->GetOwner()->GetTransform();
		meshBound.Transform(cameraSpace);
		if (frustum.Cull(meshBound))
		{
			itr->m_bVisible = true;
		}
		else
		{
			itr->m_bVisible = false;
		}
	}
}

void SceneGraph::Render()
{
	// TODO: update per frame cbuffer

	m_pVSCBuffer->BindToRenderer();
	m_pPSCBuffer->BindToRenderer();

	int count = 0; //
	for (auto itr : m_tree)
	{
		if (itr->m_bVisible) count++; //

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->VS.m_data;
		ptr->WorldTransform = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *itr->m_pTransform;
		ptr->Transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * *itr->m_pTransform;
		m_pVSCBuffer->Update();

		PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER* ptr2 = (PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER*) m_pPSCBuffer->PS.m_data;
		ptr2->material.vSpecular = itr->m_pMeshData->m_Material.GetSpecular();
		ptr2->material.fShininess = itr->m_pMeshData->m_Material.GetShininess();
		m_pPSCBuffer->Update();

		itr->m_pMeshData->RenderUsingPassAt(0);
	}
	static wchar_t s[64];
	swprintf(s, 64, L"Drawing: %i\n", count);
	OutputDebugStringW(s);

	ID3D11ShaderResourceView* null[2] = { nullptr };
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, 2, null);
}

void SceneGraph::ShadowMapGeneration()
{
	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);
		if (currLight->IsCastShadow())
		{
			D3D11Renderer::GetInstance()->m_pD3D11Context->ClearDepthStencilView(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex())->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			for (auto itr : m_tree)
			{
				VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->VS.m_data;
				ptr->WorldTransform = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *itr->m_pTransform;
				ptr->Transform = currLight->GetOwner()->GetComponent<CameraComponent>()->GetPVMatrix() * *itr->m_pTransform;
				m_pVSCBuffer->Update();

				RenderPass* shadowPass = new RenderPass;
				shadowPass->SetVertexShader("Shaders/VS_vertex1P1N1T1UV.hlsl");
				shadowPass->SetPixelShader(nullptr);
				shadowPass->SetBlendState(State::NULL_STATE);
				shadowPass->SetDepthStencilView(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex())->GetDSV());
				shadowPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);

				itr->m_pMeshData->RenderUsingPass(shadowPass);

				delete shadowPass;
			}
		}
	}
}

void SceneGraph::RENDER_DEBUG_DRAWING()
{
#ifdef _DEBUG
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetDepthStencilState((ID3D11DepthStencilState*) State::GetState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS), 1);
	D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState((ID3D11RasterizerState*) State::GetState(State::WIREFRAME_RS));
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

