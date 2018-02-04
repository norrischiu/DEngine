#include "DEBUG_RENDERER.h"
#include "../GlobalInclude.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

DEBUG_RENDERER* DEBUG_RENDERER::m_pInstance = 0;

DEBUG_RENDERER::DEBUG_RENDERER()
	: m_vDbgMeshs(0)
	, m_vRenderFlags(0)
{
//	m_m2DProjection = Matrix4::OrthographicProjection(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f);
//
//	m_3DRenderPass.SetBlendState(State::NULL_STATE);
//	m_3DRenderPass.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	m_3DRenderPass.SetDepthStencilState(State::NULL_STATE);
//#ifdef D3D11
//	m_3DRenderPass.SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
//#endif
//
//	Vertex1P1UV* pVertices = new Vertex1P1UV[4];
//	unsigned int pIndices[6] =
//	{
//		0, 1, 2,
//		3, 2, 1,
//	};
//	pVertices[0].m_pos = Vector3(-382.0, -503.0, 0.0);
//	pVertices[0].m_UV[0] = 0.0f;
//	pVertices[0].m_UV[1] = 0.0f;
//	pVertices[1].m_pos = Vector3(385.0, -503.0, 0.0);
//	pVertices[1].m_UV[0] = 1.0f;
//	pVertices[1].m_UV[1] = 0.0f;
//	pVertices[2].m_pos = Vector3(-382.0, -513.0, 0.0);
//	pVertices[2].m_UV[0] = 0.0f;
//	pVertices[2].m_UV[1] = 1.0f;
//	pVertices[3].m_pos = Vector3(385.0, -513.0, 0.0);
//	pVertices[3].m_UV[0] = 1.0f;
//	pVertices[3].m_UV[1] = 1.0f;
//	hpMeter = new MeshData(pVertices, 4, pIndices, 6, sizeof(Vertex1P1UV));
//	RenderPass* pass = new RenderPass;
//	pass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
//	pass->SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
//	pass->SetRasterizerState(State::CULL_NONE_RS);
//	pass->SetBlendState(State::NULL_STATE);
//	pass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	pass->SetDepthStencilState(State::NULL_STATE);
//#ifdef D3D11
//	pass->SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
//#endif
//	Handle hTexture(sizeof(Texture));
//	new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, "hpmeter.dds");
//	pass->AddTexture(hTexture);
//	hpMeter->m_Material.GetRenderTechnique()->AddPass(pass);
//
//	pVertices[0].m_pos = Vector3(-480.0, -486.0, 0.0);
//	pVertices[1].m_pos = Vector3(480.0, -486.0, 0.0);
//	pVertices[2].m_pos = Vector3(-480.0, -524.0, 0.0);
//	pVertices[3].m_pos = Vector3(480.0, -524.0, 0.0);
//	hpMeterBorder = new MeshData(pVertices, 4, pIndices, 6, sizeof(Vertex1P1UV));
//	RenderPass* boarderPass = new RenderPass;
//	boarderPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
//	boarderPass->SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
//	boarderPass->SetRasterizerState(State::CULL_NONE_RS);
//	boarderPass->SetBlendState(State::ALPHA_BS);
//	boarderPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	boarderPass->SetDepthStencilState(State::NULL_STATE);
//#ifdef D3D11
//	boarderPass->SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
//#endif
//	Handle hTexture2(sizeof(Texture));
//	new (hTexture2) Texture(Texture::SHADER_RESOURCES, 1, "hpmeter_border.dds");
//	boarderPass->AddTexture(hTexture2);
//	hpMeterBorder->m_Material.GetRenderTechnique()->AddPass(boarderPass);
//
//	pVertices[0].m_pos = Vector3(-780.0, 470.0, 0.0);
//	pVertices[1].m_pos = Vector3(-880.0, 470.0, 0.0);
//	pVertices[2].m_pos = Vector3(-780.0, 460.0, 0.0);
//	pVertices[3].m_pos = Vector3(-880.0, 460.0, 0.0);
//	staminaMeter = new MeshData(pVertices, 4, pIndices, 6, sizeof(Vertex1P1UV));
//	RenderPass* staminaPass = new RenderPass;
//	staminaPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
//	staminaPass->SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
//	staminaPass->SetRasterizerState(State::CULL_NONE_RS);
//	staminaPass->SetBlendState(State::NULL_STATE);
//	staminaPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	staminaPass->SetDepthStencilState(State::NULL_STATE);
//#ifdef D3D11
//	staminaPass->SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
//#endif
//	Handle hStaminaTexture(sizeof(Texture));
//	new (hStaminaTexture) Texture(Texture::SHADER_RESOURCES, 1, "stamina_meter.dds");
//	staminaPass->AddTexture(hStaminaTexture);
//	staminaMeter->m_Material.GetRenderTechnique()->AddPass(staminaPass);
//
//	pVertices[0].m_pos = Vector3(-730.0, 490.0, 0.0);
//	pVertices[1].m_pos = Vector3(-880.0, 490.0, 0.0);
//	pVertices[2].m_pos = Vector3(-730.0, 480.0, 0.0);
//	pVertices[3].m_pos = Vector3(-880.0, 480.0, 0.0);
//	playerHpMeter = new MeshData(pVertices, 4, pIndices, 6, sizeof(Vertex1P1UV));
//	RenderPass* playerHpPass = new RenderPass;
//	playerHpPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
//	playerHpPass->SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
//	playerHpPass->SetRasterizerState(State::CULL_NONE_RS);
//	playerHpPass->SetBlendState(State::NULL_STATE);
//	playerHpPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	playerHpPass->SetDepthStencilState(State::NULL_STATE);
//#ifdef D3D11
//	playerHpPass->SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
//#endif
//	Handle hPlayerHpTexture(sizeof(Texture));
//	new (hPlayerHpTexture) Texture(Texture::SHADER_RESOURCES, 1, "player_hpmeter.dds");
//	playerHpPass->AddTexture(hPlayerHpTexture);
//	playerHpMeter->m_Material.GetRenderTechnique()->AddPass(playerHpPass);
//
//	boosHPWidth = 800.0f;
//	PlayerStaminaWidth = 100.0f;
//	PlayerHpWidth = 150.0f;
}

};