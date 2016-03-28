#include "SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Graphics\Animation\Skeleton.h"
#include "Graphics\Animation\AnimationController.h"
#include "Object\CameraComponent.h"
#include "Light\LightManager.h"
#include "Graphics\Render\State.h"
#include "Graphics\Render\RenderPass.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Graphics\Render\PSPerMaterialCBuffer.h"
#include "Graphics\Render\VSMatrixPaletteCBuffer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"
#include "Graphics\terrain\terrain.h"
#include "Math\Plane.h"

namespace DE
{

SceneGraph* SceneGraph::m_pInstance;

SceneGraph::SceneGraph()
{
	m_pVSCBuffer = new VSPerObjectCBuffer;
	m_pPSCBuffer = new PSPerMaterialCBuffer;
	m_pMatrixPalette = new VSMatrixPaletteCBuffer;

	shadowPass = new RenderPass;
	shadowPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV.hlsl");
	shadowPass->SetPixelShader(nullptr);
	shadowPass->SetBlendState(State::NULL_STATE);
	shadowPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
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
	m_pMatrixPalette->BindToRenderer();
	m_pPSCBuffer->BindToRenderer();

	int count = 0; //
	for (auto itr : m_tree)
	{
		if (itr->m_bVisible) count++; //

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;
		ptr->WorldTransform = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *itr->m_pTransform;
		ptr->WVPTransform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * *itr->m_pTransform;
		m_pVSCBuffer->Update();

		AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
		if (anim != nullptr)
		{
			Skeleton* skel = anim->m_skeleton;
			VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_pMatrixPalette->m_Memory._data;
			for (auto itr : anim->m_animationSets)
			{
				if (itr.second->isActive())
				{
					for (int index = 0; index < itr.second->m_vAnimations.size(); ++index)
					{
						palette->mSkinning[index] = skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
					}
				}
			}
			m_pMatrixPalette->Update();
		}

		// For terrain
		HSDSPerFrameCBuffer m_pHSDSCBuffer;
		m_pHSDSCBuffer.BindToRenderer();
		HSDSPerFrameCBuffer::HSDS_CBUFFER* pHSDSCBuffer = (HSDSPerFrameCBuffer::HSDS_CBUFFER*) m_pHSDSCBuffer.m_Memory._data;
		pHSDSCBuffer->gViewProj = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix();
		D3D11Renderer::GetInstance()->GetCamera()->GetFrustum();
		for (int i = 0; i < 6; i++)
		{
			Vector3 planeNormal = D3D11Renderer::GetInstance()->GetCamera()->GetFrustum().GetPlanes()[i].GetNormal();
			planeNormal.SetW(1.0f);
			planeNormal.Transform(*itr->GetOwner()->GetTransform() * D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix().Inverse());
			pHSDSCBuffer->gWorldFrustumPlanes[i] = planeNormal;
		}
		pHSDSCBuffer->gEyePosW = D3D11Renderer::GetInstance()->GetCamera()->GetPosition();
		pHSDSCBuffer->gView = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix();
		pHSDSCBuffer->gTexelCellSpaceU = 1.0f / 256.0f;
		pHSDSCBuffer->gTexelCellSpacev = 1.0f / 256.0f;
		m_pHSDSCBuffer.Update();

		PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER* ptr2 = (PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER*) m_pPSCBuffer->m_Memory._data;
		ptr2->material.vSpecular = itr->m_pMeshData->m_Material.GetSpecular();
		ptr2->material.fShininess = itr->m_pMeshData->m_Material.GetShininess();
		m_pPSCBuffer->Update();

		itr->Draw();
	}

	
	static wchar_t s[64];
	swprintf(s, 64, L"Drawing: %i\n", count);
	OutputDebugStringW(s);
	
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
				VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;
				ptr->WVPTransform = currLight->GetOwner()->GetComponent<CameraComponent>()->GetPVMatrix() * *itr->m_pTransform;
				m_pVSCBuffer->Update();

				shadowPass->SetDepthStencilView(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex())->GetDSV());
				itr->m_pMeshData->RenderUsingPass(shadowPass);
			}
		}
	}
}

void SceneGraph::RENDER_DEBUG_DRAWING()
{
#ifdef _DEBUG

	m_pVSCBuffer->BindToRenderer();

	VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;

	/*RenderPass* pass = new RenderPass;
	pass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	pass->SetPixelShader("../DEngine/Shaders/PS_red.hlsl");
	pass->SetBlendState(State::NULL_STATE);
	pass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
	pass->SetRasterizerState(State::CULL_NONE_RS);
	pass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);*/
	for (auto itr : DEBUG_DRAWING_TREE)
	{
		ptr->WVPTransform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * *itr->m_pTransform;
		m_pVSCBuffer->Update();

		//itr->m_pMeshData->RenderUsingPass(pass);
		itr->m_pMeshData->Render();
	}
	//delete pass;

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

};