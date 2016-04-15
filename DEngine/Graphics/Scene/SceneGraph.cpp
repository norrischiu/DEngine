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
	m_ShadowPass = new RenderPass;
	m_ShadowPass->SetPixelShader(nullptr);
	m_ShadowPass->SetBlendState(State::NULL_STATE);
	m_ShadowPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
}

void SceneGraph::FrustumCulling(Frustum frustum)
{
	for (auto itr : m_tree)
	{
		AABB meshBound = itr->m_pMeshData->GetBoundingBox();
		AABB cache = meshBound;
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

	m_VSCBuffer.BindToRenderer();
	m_MatrixPalette.BindToRenderer();
	m_PSCBuffer.BindToRenderer();

	int count = 0; //
	for (auto itr : m_tree)
	{
		if (itr->m_bVisible) count++; //

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;
		ptr->WorldTransform = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *itr->m_pTransform;
		ptr->WVPTransform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * *itr->m_pTransform;
		m_VSCBuffer.Update();

		AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
		if (anim != nullptr)
		{
			Skeleton* skel = anim->m_skeleton;
			VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_MatrixPalette.m_Memory._data;
			for (auto itr : anim->m_animationSets)
			{
				if (itr.second->isActive())
				{
					for (int index = 0; index < itr.second->m_vAnimations.size(); ++index)
					{
						palette->mSkinning[index] = *skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
					}
				}
			}
			m_MatrixPalette.Update();
		}

		// For terrain
		/*m_HSDSCBuffer.BindToRenderer();
		HSDSPerFrameCBuffer::HSDS_CBUFFER* pHSDSCBuffer = (HSDSPerFrameCBuffer::HSDS_CBUFFER*) m_HSDSCBuffer.m_Memory._data;
		pHSDSCBuffer->gViewProj = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix();
		for (int i = 0; i < 6; i++)
		{
			Plane plane = D3D11Renderer::GetInstance()->GetCamera()->GetFrustum().GetPlanes()[i];
			plane.Transform(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix().Inverse());
			pHSDSCBuffer->gWorldFrustumPlanes[i] = plane.GetNormal();
		}
		pHSDSCBuffer->gEyePosW = D3D11Renderer::GetInstance()->GetCamera()->GetPosition();
		pHSDSCBuffer->gTexelCellSpaceU = 1.0f / 256.0f;
		pHSDSCBuffer->gTexelCellSpacev = 1.0f / 256.0f;
		m_HSDSCBuffer.Update();*/

		PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER* ptr2 = (PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER*) m_PSCBuffer.m_Memory._data;
		ptr2->material.vSpecular = itr->m_pMeshData->m_Material.GetSpecular();
		ptr2->material.fShininess = itr->m_pMeshData->m_Material.GetShininess();
		m_PSCBuffer.Update();

		itr->Draw();
	}
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
				// only cast shadow for skeletal mesh for now
				AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
				if (anim != nullptr)
				{
					VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;
					ptr->WVPTransform = currLight->GetOwner()->GetComponent<CameraComponent>()->GetPVMatrix() * *itr->m_pTransform;
					m_VSCBuffer.Update();

					m_ShadowPass->SetDepthStencilView(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex())->GetDSV());

					Skeleton* skel = anim->m_skeleton;
					VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_MatrixPalette.m_Memory._data;
					for (auto itr : anim->m_animationSets)
					{
						if (itr.second->isActive())
						{
							for (int index = 0; index < itr.second->m_vAnimations.size(); ++index)
							{
								palette->mSkinning[index] = *skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
							}
						}
					}
					m_MatrixPalette.Update();
				
					// TODO: separete skeletal and static mesh
					//m_ShadowPass->SetVertexShader(itr->m_pMeshData->m_Material.GetRenderTechnique()->m_vRenderPasses[0]->GetVertexShader());
					m_ShadowPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV4J.hlsl");
					itr->m_pMeshData->RenderUsingPass(m_ShadowPass);
				}
			}
		}
	}
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