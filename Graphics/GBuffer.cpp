#include "GBuffer.h"
#include "Light\LightManager.h"
#include "Light\PointLightComponent.h"
#include "Light\SpotLightComponent.h"
#include "Render\RenderPass.h"
#include "Render\State.h"
#include "D3D11Renderer.h"
#include "Render\PSPerLightCBuffer.h"
#include "MeshData.h"
#include "Render\Texture.h"

GBuffer::GBuffer()
{
	m_pPSCBuffer = new PSPerLightCBuffer;
	m_pVSCBuffer = new VSPerObjectCBuffer; // use static

	pointLightMesh = new MeshData(LightManager::GetInstance()->GetPointLightVertices(), 8, (unsigned int*)LightManager::GetInstance()->GetPointLightIndices(), 36);
	spotLightMesh = new MeshData(LightManager::GetInstance()->GetSpotLightVertices(), 5, (unsigned int*)LightManager::GetInstance()->GetSpotLightIndices(), 18);

	StencilingPass = new RenderPass;
	StencilingPass->SetVertexShader("Shaders/VS_vertex1P.hlsl");
	StencilingPass->SetPixelShader(nullptr);
	StencilingPass->SetRasterizerState(State::CULL_NONE_RS);
	StencilingPass->SetDepthStencilState(State::GBUFFER_STENCIL_CHECK_DSS);
	StencilingPass->SetDepthStencilView(D3D11Renderer::GetInstance()->m_depth->GetDSV());

	LightingPass = new RenderPass;
	LightingPass->SetVertexShader("Shaders/VS_gbuffer.hlsl");
	LightingPass->SetPixelShader("Shaders/PS_phong_gbuffer.hlsl");
	LightingPass->SetRasterizerState(State::CULL_FRONT_RS);
	LightingPass->SetDepthStencilState(State::DISABLE_DEPTH_STENCIL_DSS);
	LightingPass->SetBlendState(State::ADDITIVE_BS);
	LightingPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
	LightingPass->SetDepthStencilView(D3D11Renderer::GetInstance()->m_depthReadOnly->GetDSV());

	LightingPass->AddTexture(D3D11Renderer::GetInstance()->m_textures[0]);
	LightingPass->AddTexture(D3D11Renderer::GetInstance()->m_textures[1]);
	LightingPass->AddTexture(D3D11Renderer::GetInstance()->m_depth);
}

void GBuffer::LightStencilCheck()
{	
	m_pVSCBuffer->BindToRenderer();

	// Set stencil buffer
	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->VS.m_data;
		Matrix4 scale, trans, rot;
		scale.CreateScale(currLight->GetRadius());
		trans.CreateTranslation(currLight->GetPosition());
		if (currLight->GetType() == LightComponent::SPOT)
		{
			SpotLightComponent* currSpotlight = (SpotLightComponent*) currLight;
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
		ptr->Transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * trans * rot * scale;
		m_pVSCBuffer->Update();

		switch (currLight->GetType())
		{
			case LightComponent::POINT:
				pointLightMesh->RenderUsingPass(StencilingPass);
				break;
			case LightComponent::SPOT:
				spotLightMesh->RenderUsingPass(StencilingPass);
				break;
		}
	}

	D3D11Renderer::GetInstance()->UnbindRenderTargets();
}

void GBuffer::Render()
{
	//LightStencilCheck();

	m_pPSCBuffer->BindToRenderer();
	m_pVSCBuffer->BindToRenderer();

	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);

		// Update VS cbuffer
		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->VS.m_data;
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
		ptr->Transform = D3D11Renderer::GetInstance()->GetCamera()->GetPVMatrix() * trans * rot * scale;
		m_pVSCBuffer->Update();

		// Update PS cbuffer
		PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER* ptr2 = (PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER*) m_pPSCBuffer->PS.m_data;
		ptr2->light.vColor = currLight->GetColor();
		ptr2->light.fIntensity = currLight->GetIntensity();
		ptr2->light.fRadius = currLight->GetRadius();
		ptr2->light.iType = currLight->GetType();
		Vector3 pos = currLight->GetPosition();
		pos.Transform(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());
		ptr2->light.vPos = pos;
		ptr2->light.bIsCastShadow = currLight->IsCastShadow();
		ptr2->mClipToView = D3D11Renderer::GetInstance()->GetCamera()->GetPerspectiveMatrix().Inverse();
		ptr2->mViewToClip = D3D11Renderer::GetInstance()->GetCamera()->GetPerspectiveMatrix();
		if (currLight->GetType() == LightComponent::SPOT)
		{
			Vector3 dir = currLight->GetDirection();
			dir.Transform(D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix());
			ptr2->light.vDir = dir.Normalize();
			ptr2->light.fInnerAngle = ((SpotLightComponent*)currLight)->GetInnerAngle();
			ptr2->light.fOuterAngle = ((SpotLightComponent*)currLight)->GetOuterAngle();
		}
		if (currLight->IsCastShadow())
		{
			CameraComponent* lightCamera = currLight->GetOwner()->GetComponent<CameraComponent>();
			ptr2->light.mWorldToLightClip = lightCamera->GetPVMatrix();
			ptr2->light.mLightClipToView = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix() * lightCamera->GetPVMatrix().Inverse();
			ptr2->mViewToWorld = D3D11Renderer::GetInstance()->GetCamera()->GetViewMatrix().Inverse();
			LightingPass->AddTexture(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex()));
		}
		m_pPSCBuffer->Update();

		switch (currLight->GetType())
		{
			case LightComponent::POINT:
				pointLightMesh->RenderUsingPass(StencilingPass);
				D3D11Renderer::GetInstance()->UnbindRenderTargets();
				pointLightMesh->RenderUsingPass(LightingPass);
				D3D11Renderer::GetInstance()->m_pD3D11Context->ClearDepthStencilView(D3D11Renderer::GetInstance()->m_depth->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
				D3D11Renderer::GetInstance()->UnbindPSShaderResources(4);
				break;
			case LightComponent::SPOT:
				spotLightMesh->RenderUsingPass(StencilingPass);
				D3D11Renderer::GetInstance()->UnbindRenderTargets();
				spotLightMesh->RenderUsingPass(LightingPass);
				D3D11Renderer::GetInstance()->m_pD3D11Context->ClearDepthStencilView(D3D11Renderer::GetInstance()->m_depth->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
				D3D11Renderer::GetInstance()->UnbindPSShaderResources(4);
				break;
		}
		if (currLight->IsCastShadow())
		{
			LightingPass->PopTexture();
		}
	}

	// Unbind the resources
	D3D11Renderer::GetInstance()->UnbindPSShaderResources(4);
	D3D11Renderer::GetInstance()->UnbindRenderTargets();
}