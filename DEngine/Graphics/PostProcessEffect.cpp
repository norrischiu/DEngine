#include "PostProcessEffect.h"
#include "D3D12Renderer.h"
#include "Render\RenderPass.h"
#include "Render\PSPerLightCBuffer.h"
#include "Render\VSPerObjectCBuffer.h"
#include "MeshData.h"
#include "..\GlobalInclude.h"

namespace DE
{

void PostProcessEffect::Initialize(Renderer* renderer)
{
	// Set vertex buffer
	Vector3* pVertexData = new Vector3[4];
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

	AmbientPass = new RenderPass;
	AmbientPass->SetVertexShader("../DEngine/Shaders/VS_gbuffer.hlsl");
	AmbientPass->SetPixelShader("../DEngine/Shaders/PS_PPE_Ambient.hlsl");
	AmbientPass->SetRasterizerState(State::CULL_BACK_RS);
	AmbientPass->SetBlendState(State::ADDITIVE_BS);
	AmbientPass->SetRenderTargets(Renderer::GetInstance()->GetCurrentBackBufferTextureAddress(), 1);
	AmbientPass->SetTextureCount(1);
	AmbientPass->ConstructPSO();
}

void PostProcessEffect::Render(Renderer* renderer)
{
	VSPerObjectCBuffer m_pVSCBuffer;
	VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer.m_Memory._data;
	ptr->WVPTransform = Matrix4::Identity;
	m_pVSCBuffer.Update();
	m_pVSCBuffer.BindToRendererWithOffset(0, 0);

	Material material;
	material.SetTextureCount(1);
	material.SetTexture(renderer->m_hTextures[0], 0);

	// Render all passes inputted
	AmbientPass->BindSignatureToRenderer(renderer);
	material.BindToRenderer(renderer);
	AmbientPass->SetRenderTargets(Renderer::GetInstance()->GetCurrentBackBufferTextureAddress(), 1);
	AmbientPass->BindToRenderer(renderer);
	fullscreenQuadMesh->Render(renderer);
}

};