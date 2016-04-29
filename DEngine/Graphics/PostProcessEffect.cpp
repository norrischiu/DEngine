#include "PostProcessEffect.h"
#include "Render\RenderPass.h"
#include "Render\State.h"
#include "Render\Texture.h"
#include "D3D11Renderer.h"
#include "Render\PSPerLightCBuffer.h"
#include "MeshData.h"
#include "../GlobalInclude.h"

namespace DE
{

PostProcessEffect::PostProcessEffect()
{
	m_pPSCBuffer = new PSPerLightCBuffer; // use static
	m_pVSCBuffer = new VSPerObjectCBuffer; // use static

											// Set vertex buffer
	Vector3* pVertexData = new Vector3[6];
	float left = -WINDOW_WIDTH / 2;
	float right = left + WINDOW_WIDTH;
	float top = WINDOW_HEIGHT / 2;
	float bottom = top - WINDOW_HEIGHT;
	pVertexData[0] = Vector3(left, top, 0);
	pVertexData[1] = Vector3(right, top, 0);
	pVertexData[2] = Vector3(left, bottom, 0);
	pVertexData[3] = Vector3(right, bottom, 0);

	// Set index buffer
	unsigned int pIndexData[6] = { 0, 1, 2, 3, 2, 1 };
	fullscreenQuadMesh = new MeshData(pVertexData, 4, pIndexData, 6);

	Handle hTexture1(sizeof(Texture));
	new (hTexture1) Texture(Texture::SHADER_RESOURCES | Texture::RENDER_TARGET);
	m_texture = (Texture*)hTexture1.Raw();
	Handle hTexture2(sizeof(Texture));
	new (hTexture2) Texture(Texture::SHADER_RESOURCES | Texture::RENDER_TARGET);
	m_texture2 = (Texture*)hTexture2.Raw();

	RenderPass* ReflectionPass = new RenderPass;
	ReflectionPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	ReflectionPass->SetPixelShader("../DEngine/Shaders/PS_SSReflection_ppe.hlsl");
	ReflectionPass->SetRasterizerState(State::CULL_BACK_RS);
	ReflectionPass->SetBlendState(State::NULL_STATE);
	ReflectionPass->SetRenderTargets(&m_texture->GetRTV(), 1);

	RenderPass* BlurHPass = new RenderPass;
	BlurHPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	BlurHPass->SetPixelShader("../DEngine/Shaders/PS_PPE_BlurH.hlsl");
	BlurHPass->SetRasterizerState(State::CULL_BACK_RS);
	BlurHPass->SetBlendState(State::NULL_STATE);
	BlurHPass->SetRenderTargets(&m_texture2->GetRTV(), 1);

	RenderPass* BlurVPass = new RenderPass;
	BlurVPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	BlurVPass->SetPixelShader("../DEngine/Shaders/PS_PPE_BlurV.hlsl");
	BlurVPass->SetRasterizerState(State::CULL_BACK_RS);
	BlurVPass->SetBlendState(State::ALPHA_BS);
	BlurVPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);

	RenderPass* AmbientPass = new RenderPass;
	AmbientPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	AmbientPass->SetPixelShader("../DEngine/Shaders/PS_PPE_Ambient.hlsl");
	AmbientPass->SetRasterizerState(State::CULL_BACK_RS);
	AmbientPass->SetBlendState(State::ADDITIVE_BS);
	AmbientPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);

	ReflectionPass->AddTexture(D3D11Renderer::GetInstance()->m_backbuffer);
	ReflectionPass->AddTexture(D3D11Renderer::GetInstance()->m_hTextures[1]);
	ReflectionPass->AddTexture(D3D11Renderer::GetInstance()->m_depth);
	BlurHPass->AddTexture(m_texture);
	BlurVPass->AddTexture(m_texture2);
	AmbientPass->AddTexture(D3D11Renderer::GetInstance()->m_hTextures[0]); // diffuse

	fullscreenQuadMesh->m_Material.AddPassToTechnique(ReflectionPass);
	fullscreenQuadMesh->m_Material.AddPassToTechnique(BlurHPass);
	fullscreenQuadMesh->m_Material.AddPassToTechnique(BlurVPass);
	fullscreenQuadMesh->m_Material.AddPassToTechnique(AmbientPass);
}

void PostProcessEffect::Render()
{
	m_pPSCBuffer->BindToRenderer();
	m_pVSCBuffer->BindToRenderer();

	// Update VS cbuffer
	VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;
	ptr->WVPTransform = Matrix4::OrthographicProjection(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f);
	m_pVSCBuffer->Update();

	// Update PS cbuffer
	PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER* ptr2 = (PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER*) m_pPSCBuffer->m_Memory._data;
	ptr2->mClipToView = D3D11Renderer::GetInstance()->GetCamera()->GetPerspectiveMatrix().Inverse();
	ptr2->mViewToClip = D3D11Renderer::GetInstance()->GetCamera()->GetPerspectiveMatrix();
	m_pPSCBuffer->Update();

	// Render all passes inputted
	fullscreenQuadMesh->Render();

	// Unbind the resources
	D3D11Renderer::GetInstance()->UnbindPSShaderResources(4);
	D3D11Renderer::GetInstance()->UnbindRenderTargets();
}

};