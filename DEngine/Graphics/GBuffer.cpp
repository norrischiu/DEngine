#include "GBuffer.h"
#include "Light\LightManager.h"
#include "Light\PointLightComponent.h"
#include "Light\SpotLightComponent.h"
#include "Render\RenderPass.h"
#include "Render\State.h"
#include "D3D11Renderer.h"
#include "D3D12Renderer.h"
#include "Render\PSPerLightCBuffer.h"
#include "MeshData.h"
#include "Render\Texture.h"

namespace DE
{

void GBuffer::InitializeMeshAndRenderPass()
{
#ifdef D3D12
	D3D12Renderer* renderer = (D3D12Renderer*)D3DRenderer::GetInstance();
#elif defined D3D11
	D3D11Renderer* renderer = (D3D11Renderer*)D3DRenderer::GetInstance();
#endif
	pointLightMesh = new MeshData(LightManager::GetInstance()->GetPointLightVertices(), 8, (unsigned int*)LightManager::GetInstance()->GetPointLightIndices(), 36);
	spotLightMesh = new MeshData(LightManager::GetInstance()->GetSpotLightVertices(), 5, (unsigned int*)LightManager::GetInstance()->GetSpotLightIndices(), 18);

	StencilingPass = new RenderPass;
	StencilingPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	StencilingPass->SetPixelShader(nullptr);
	StencilingPass->SetRasterizerState(State::CULL_NONE_RS);
	StencilingPass->SetBlendState(State::DEFAULT_BS);
	StencilingPass->SetDepthStencilState(State::GBUFFER_STENCIL_CHECK_DSS);
	StencilingPass->SetDepthStencilView(((D3D12Renderer*)D3DRenderer::GetInstance())->m_depth);
#ifdef D3D12
	StencilingPass->ConstructPSO();
#endif

	LightingPass = new RenderPass;
	LightingPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	LightingPass->SetPixelShader("../DEngine/Shaders/PS_phong_gbuffer.hlsl");
	LightingPass->SetRasterizerState(State::CULL_FRONT_RS);
	LightingPass->SetDepthStencilState(State::DISABLE_DEPTH_STENCIL_DSS);
	LightingPass->SetBlendState(State::ADDITIVE_BS);
	LightingPass->SetRenderTargets(((D3D12Renderer*)D3DRenderer::GetInstance())->GetCurrentBackBufferTextureAddress(), 1);
	LightingPass->SetDepthStencilView(((D3D12Renderer*)D3DRenderer::GetInstance())->m_depthReadOnly);
	LightingPass->AddTexture(renderer->m_hTextures[0]);
	LightingPass->AddTexture(renderer->m_hTextures[1]);
	LightingPass->AddTexture(renderer->m_depth);
#ifdef D3D12
	LightingPass->ConstructPSO();
#endif
}

void GBuffer::Render()
{
	m_pPSCBuffer.BindToRenderer();
	m_pVSCBuffer.BindToRenderer();

	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
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
#ifdef D3D12
		ptr->WVPTransform = ((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPVMatrix() * trans * rot * scale;
#elif defined D3D11
		ptr->WVPTransform = ((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPVMatrix() * trans * rot * scale;
#endif
		m_pVSCBuffer.Update();
		m_pVSCBuffer.BindToRendererWithOffset(0, i); // 0 is VS per object

		// Update PS cbuffer
		PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER* ptr2 = (PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER*) m_pPSCBuffer.m_Memory._data;
		ptr2->light.vColor = currLight->GetColor();
		ptr2->light.fIntensity = currLight->GetIntensity();
		ptr2->light.fRadius = currLight->GetRadius();
		ptr2->light.iType = currLight->GetType();
		Vector3 pos = currLight->GetPosition();
#ifdef D3D12
		ptr2->mClipToView = ((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPerspectiveMatrix().Inverse();
		ptr2->mViewToClip = ((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPerspectiveMatrix();
		pos.Transform(((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix());
#elif defined D3D11
		ptr2->mClipToView = ((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPerspectiveMatrix().Inverse();
		ptr2->mViewToClip = ((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetPerspectiveMatrix();
		pos.Transform(((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix());
#endif
		ptr2->light.vPos = pos;
		ptr2->light.bIsCastShadow = currLight->IsCastShadow();

		if (currLight->GetType() == LightComponent::SPOT)
		{
			Vector3 dir = currLight->GetDirection();
#ifdef D3D12
			dir.Transform(((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix());
#elif defined D3D11
			dir.Transform(((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix());
#endif
			ptr2->light.vDir = dir.Normalize();
			ptr2->light.fInnerAngle = ((SpotLightComponent*)currLight)->GetInnerAngle();
			ptr2->light.fOuterAngle = ((SpotLightComponent*)currLight)->GetOuterAngle();
		}
		if (currLight->IsCastShadow())
		{
			CameraComponent* lightCamera = currLight->GetOwner()->GetComponent<CameraComponent>();
			ptr2->light.mWorldToLightClip = lightCamera->GetPVMatrix();
#ifdef D3D12
			ptr2->light.mLightClipToView = ((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix() * lightCamera->GetPVMatrix().Inverse();
			ptr2->mViewToWorld = ((D3D12Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix().Inverse();
#elif defined D3D11
			ptr2->light.mLightClipToView = ((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix() * lightCamera->GetPVMatrix().Inverse();
			ptr2->mViewToWorld = ((D3D11Renderer*)D3DRenderer::GetInstance())->GetCamera()->GetViewMatrix().Inverse();
#endif
			LightingPass->AddTexture(LightManager::GetInstance()->GetShadowMap(currLight->GetShadowMapIndex()));
		}
		m_pPSCBuffer.Update();
		m_pPSCBuffer.BindToRendererWithOffset(2, i);

		switch (currLight->GetType())
		{
		case LightComponent::POINT:
			pointLightMesh->RenderUsingPass(StencilingPass);
#ifdef D3D12
			LightingPass->SetRenderTargets(((D3D12Renderer*)D3DRenderer::GetInstance())->GetCurrentBackBufferTextureAddress(), 1);
			pointLightMesh->RenderUsingPass(LightingPass);
#elif defined D3D11
			((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
			pointLightMesh->RenderUsingPass(LightingPass);
			((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->ClearDepthStencilView(((D3D11Renderer*)D3DRenderer::GetInstance())->m_depth->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
			((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindPSShaderResources(4);
#endif
			break;
		case LightComponent::SPOT:
			spotLightMesh->RenderUsingPass(StencilingPass);
#ifdef D3D12
			LightingPass->SetRenderTargets(((D3D12Renderer*)D3DRenderer::GetInstance())->GetCurrentBackBufferTextureAddress(), 1);
			spotLightMesh->RenderUsingPass(LightingPass);
#elif defined D3D11
			((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
			spotLightMesh->RenderUsingPass(LightingPass);
			((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->ClearDepthStencilView(((D3D11Renderer*)D3DRenderer::GetInstance())->m_depth->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
			((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindPSShaderResources(4);
#endif
			break;
		}
		if (currLight->IsCastShadow())
		{
			LightingPass->PopTexture();
		}
	}

	// Unbind the resources
#ifdef D3D11
	((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindPSShaderResources(4);
	((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
#endif
}

};