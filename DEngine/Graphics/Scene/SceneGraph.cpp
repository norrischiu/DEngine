#include "SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Graphics\Animation\Skeleton.h"
#include "Graphics\Animation\AnimationController.h"
#include "Object\CameraComponent.h"
#include "Light\LightManager.h"
#include "Graphics\Render\State.h"
#include "Graphics\Render\RenderPass.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Graphics\Render\PSPerMaterialCBuffer.h"
#include "Graphics\Render\VSMatrixPaletteCBuffer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"
#include "Math\Plane.h"

namespace DE
{

SceneGraph* SceneGraph::m_pInstance;

SceneGraph::SceneGraph()
	: m_tree(0)
	, DEBUG_DRAWING_TREE(0)
{
	m_staticMeshGeometryPass = new RenderPass();
	m_staticMeshGeometryPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV.hlsl");
	m_staticMeshGeometryPass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_DiffuseSpecularBump_deferred.hlsl");
	m_staticMeshGeometryPass->SetTextureCount(3);
	m_staticMeshGeometryPass->SetBlendState(State::DEFAULT_BS);
	m_staticMeshGeometryPass->SetRenderTargets(Renderer::GetInstance()->m_pRTV, 2);
	m_staticMeshGeometryPass->SetDepthStencilView(reinterpret_cast<Texture*>(Renderer::GetInstance()->m_depth.Raw()));
	m_staticMeshGeometryPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	m_staticMeshGeometryPass->ConstructPSO();

	m_skeletalMeshGeometryPass = new RenderPass();
	m_skeletalMeshGeometryPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV4J.hlsl");
	m_skeletalMeshGeometryPass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_DiffuseSpecularBump_deferred.hlsl");
	m_skeletalMeshGeometryPass->SetTextureCount(3);
	m_skeletalMeshGeometryPass->SetBlendState(State::DEFAULT_BS);
	m_skeletalMeshGeometryPass->SetRenderTargets(Renderer::GetInstance()->m_pRTV, 2);
	m_skeletalMeshGeometryPass->SetDepthStencilView(reinterpret_cast<Texture*>(Renderer::GetInstance()->m_depth.Raw()));
	m_skeletalMeshGeometryPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	m_skeletalMeshGeometryPass->ConstructPSO();

	m_ShadowPass = new RenderPass;
	m_ShadowPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV4J.hlsl");
	m_ShadowPass->SetPixelShader(nullptr);
	m_ShadowPass->SetBlendState(State::NULL_STATE);
	m_ShadowPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	m_ShadowPass->ConstructPSO();
}

void SceneGraph::FrustumCulling(CameraComponent* camera)
{
	const unsigned int size = m_tree.Size();
	for (int i = 0; i < size; ++i)
	{
		AABB meshBound = m_tree[i]->GetMeshData()->GetBoundingBox();
		AABB cache = meshBound;
		Matrix4 cameraSpace = camera->GetViewMatrix() * *m_tree[i]->GetOwner()->GetTransform();
		meshBound.Transform(cameraSpace);
		m_tree[i]->SetVisibility(camera->GetFrustum().Cull(meshBound));
	}
}

void SceneGraph::Render(Renderer* renderer)
{
	m_skeletalMeshGeometryPass->BindSignatureToRenderer(renderer); // TODO: separate/sort static and skeletal mesh rendering

	int count = 0; //
	const unsigned int size = m_tree.Size();
	for (int i = 0; i < size; ++i)
	{
		MeshComponent* itr = m_tree[i];
		if (itr->IsVisible()) count++; //

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;
		ptr->WorldTransform = renderer->GetCamera()->GetViewMatrix() * *itr->GetTransform();
		ptr->WVPTransform = renderer->GetCamera()->GetPVMatrix() * *itr->GetTransform();
		m_VSCBuffer.Update();
		m_VSCBuffer.BindToRendererWithOffset(0, i); // 0 is VS per object

		AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
		if (anim != nullptr)
		{
			Skeleton* skel = itr->GetOwner()->GetComponent<Skeleton>();
			VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_MatrixPalette.m_Memory._data;
			const int jointCount = anim->GetAnimationSetCount();
			anim->GetAnimationSets().ForEachItem([skel, palette](AnimationSet* item)
			{
				if (item->isActive())
				{
					for (int index = 0; index < skel->GetJointsCount(); ++index)
					{
						palette->mSkinning[index] = *skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
					}
				}
			}); 
			m_MatrixPalette.Update();
			m_MatrixPalette.BindToRendererWithOffset(1, i); // 1 is VS matrix palatte

			m_skeletalMeshGeometryPass->BindToRenderer(renderer);
		}
		else
		{
			m_staticMeshGeometryPass->BindToRenderer(renderer);
		}

		PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER* ptr2 = (PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER*) m_PSCBuffer.m_Memory._data;
		ptr2->material.vSpecular = itr->GetMaterial()->GetSpecular();
		ptr2->material.fShininess = itr->GetMaterial()->GetShininess();
		m_PSCBuffer.Update();
		m_PSCBuffer.BindToRendererWithOffset(2, i); // 2 is PS material

		itr->Draw(renderer);
	}
}

void SceneGraph::ShadowMapGeneration(Renderer* renderer)
{
	m_ShadowPass->BindSignatureToRenderer(renderer);

	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);
		if (currLight->IsCastShadow())
		{
			Handle shadowMap = LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex());
			Renderer::GetInstance()->m_pCommandList->ClearDepthStencilView(reinterpret_cast<Texture*>(shadowMap.Raw())->GetDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
			const unsigned int size = m_tree.Size();
			for (int i = 0; i < size; ++i)
			{
				MeshComponent* itr = m_tree[i];
				// only cast shadow for skeletal mesh for now
				AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
				if (anim != nullptr)
				{
					VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;
					ptr->WVPTransform = currLight->GetOwner()->GetComponent<CameraComponent>()->GetPVMatrix() * *itr->GetTransform();
					m_VSCBuffer.Update();
					m_VSCBuffer.BindToRendererWithOffset(0, i);

					m_ShadowPass->SetDepthStencilView(reinterpret_cast<Texture*>(shadowMap.Raw()));

					Skeleton* skel = itr->GetOwner()->GetComponent<Skeleton>();
					VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_MatrixPalette.m_Memory._data;
					anim->GetAnimationSets().ForEachItem([skel, palette](AnimationSet* item)
					{
						if (item->isActive())
						{
							for (int index = 0; index < skel->GetJointsCount(); ++index)
							{
								palette->mSkinning[index] = *skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
							}
						}
					});
					m_MatrixPalette.Update();
					m_MatrixPalette.BindToRendererWithOffset(1, i);

					// TODO: separete skeletal and static mesh
					m_ShadowPass->BindToRenderer(renderer);
					itr->GetMeshData()->Render(renderer);
				}
			}		
		}
	}
}

void SceneGraph::AddComponent(MeshComponent * meshComponent)
{
	m_tree.Add(meshComponent);
}

void SceneGraph::ADD_DEBUG_DRAWING(MeshComponent * meshComponent)
{
	DEBUG_DRAWING_TREE.Add(meshComponent);
}

};