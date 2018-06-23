#include "GBuffer.h"
#include "Light\LightManager.h"
#include "Light\PointLightComponent.h"
#include "Light\SpotLightComponent.h"
#include "Render\RenderPass.h"
#include "Render\State.h"
#include "D3D12Renderer.h"
#include "Render\PSPerLightCBuffer.h"
#include "MeshData.h"
#include "Render\Texture.h"

namespace DE
{

void GBuffer::InitializeMeshAndRenderPass()
{
	Renderer* renderer = Renderer::GetInstance();
	pointLightMesh = new MeshData(LightManager::GetInstance()->GetPointLightVertices(), 8, (unsigned int*)LightManager::GetInstance()->GetPointLightIndices(), 36);
	spotLightMesh = new MeshData(LightManager::GetInstance()->GetSpotLightVertices(), 5, (unsigned int*)LightManager::GetInstance()->GetSpotLightIndices(), 18);

	StencilingPass = new RenderPass;
	StencilingPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	StencilingPass->SetPixelShader(nullptr);
	StencilingPass->SetRasterizerState(State::CULL_NONE_RS);
	StencilingPass->SetBlendState(State::DEFAULT_BS);
	StencilingPass->SetDepthStencilState(State::GBUFFER_STENCIL_CHECK_DSS);
	StencilingPass->SetDepthStencilView(reinterpret_cast<Texture*>(Renderer::GetInstance()->m_depth.Raw()));

	StencilingPass->ConstructPSO();

	LightingPass = new RenderPass;
	LightingPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	LightingPass->SetPixelShader("../DEngine/Shaders/PS_phong_gbuffer.hlsl");
	LightingPass->SetRasterizerState(State::CULL_FRONT_RS);
	LightingPass->SetDepthStencilState(State::DISABLE_DEPTH_STENCIL_DSS);
	LightingPass->SetBlendState(State::ADDITIVE_BS);
	LightingPass->SetRenderTargets(Renderer::GetInstance()->GetCurrentBackBufferTextureAddress(), 1);
	LightingPass->SetDepthStencilView(Renderer::GetInstance()->m_depthReadOnly);
	LightingPass->SetTextureCount(4);

	LightingPass->ConstructPSO();
}

void GBuffer::Render(Renderer* renderer)
{
	m_pPSCBuffer.BindToRenderer();
	m_pVSCBuffer.BindToRenderer();

	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		Material material;
		material.SetTextureCount(4);
		material.SetTexture(renderer->m_hTextures[0], 0);
		material.SetTexture(renderer->m_hTextures[1], 1);
		material.SetTexture(renderer->m_depth, 2);

		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);

		// Update VS cbuffer
		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer.m_Memory._data;
		Matrix4 scale, trans, rot;
		scale.CreateScale(currLight->GetRadius());
		trans.CreateTranslation(currLight->GetPosition());
		if (currLight->GetType() == LightComponent::SPOT)
		{
			SpotLightComponent* currSpotlight = (SpotLightComponent*)currLight;
			Matrix4 scaleX, scaleY, scaleZ, transY;
			scaleX.CreateScaleX(2.0f * currLight->GetRadius() * sin(currSpotlight->GetOuterAngle() / 2.0f));
			scaleY.CreateScaleY(currLight->GetRadius());
			scaleZ.CreateScaleZ(2.0f * currLight->GetRadius() * sin(currSpotlight->GetOuterAngle() / 2.0f));
			scale = scaleZ * scaleY * scaleX;
			transY.CreateTranslation(Vector3(0.0f, -currLight->GetRadius() / 2.0f, 0.0f));
			trans *= transY;
			Vector3 dir = currLight->GetDirection();
			dir.Normalize();
			Vector3 axis = Cross(dir, Vector3::NegativeUnitY);
			Quaternion quat(axis, asinf(axis.Length()));
			rot = quat.GetRotationMatrix();
		}
		ptr->WVPTransform = renderer->GetCamera()->GetPVMatrix() * trans * rot * scale;
		m_pVSCBuffer.Update();
		m_pVSCBuffer.BindToRendererWithOffset(0, i); // 0 is VS per object

		// Update PS cbuffer
		PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER* ptr2 = (PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER*) m_pPSCBuffer.m_Memory._data;
		ptr2->light.vColor = currLight->GetColor();
		ptr2->light.fIntensity = currLight->GetIntensity();
		ptr2->light.fRadius = currLight->GetRadius();
		ptr2->light.iType = currLight->GetType();
		Vector3 pos = currLight->GetPosition();
		ptr2->mClipToView = renderer->GetCamera()->GetPerspectiveMatrix().Inverse();
		ptr2->mViewToClip = renderer->GetCamera()->GetPerspectiveMatrix();
		pos.Transform(renderer->GetCamera()->GetViewMatrix());
		ptr2->light.vPos = pos;
		ptr2->light.bIsCastShadow = currLight->IsCastShadow();

		if (currLight->GetType() == LightComponent::SPOT)
		{
			Vector3 dir = currLight->GetDirection();
			dir.Transform(renderer->GetCamera()->GetViewMatrix());
			ptr2->light.vDir = dir.Normalize();
			ptr2->light.fInnerAngle = ((SpotLightComponent*)currLight)->GetInnerAngle();
			ptr2->light.fOuterAngle = ((SpotLightComponent*)currLight)->GetOuterAngle();
		}
		if (currLight->IsCastShadow())
		{
			CameraComponent* lightCamera = currLight->GetOwner()->GetComponent<CameraComponent>();
			ptr2->light.mWorldToLightClip = lightCamera->GetPVMatrix();
			ptr2->light.mLightClipToView = renderer->GetCamera()->GetViewMatrix() * lightCamera->GetPVMatrix().Inverse();
			ptr2->mViewToWorld = renderer->GetCamera()->GetViewMatrix().Inverse();
			material.SetTexture(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex()), 3);
		}
		else
		{
			Handle nullHandle;
			material.SetTexture(nullHandle, 3);
		}

		m_pPSCBuffer.Update();
		m_pPSCBuffer.BindToRendererWithOffset(2, i);

		MeshData* lightMesh = currLight->GetType() == LightComponent::POINT ? pointLightMesh : spotLightMesh;
		StencilingPass->BindSignatureToRenderer(renderer);
		StencilingPass->BindToRenderer(renderer);
		lightMesh->Render(renderer);
		LightingPass->SetRenderTargets(renderer->GetCurrentBackBufferTextureAddress(), 1);
		LightingPass->BindSignatureToRenderer(renderer);
		LightingPass->BindToRenderer(renderer);
		material.BindToRenderer(renderer);
		lightMesh->Render(renderer);
	}

}
};