#include "DEBUG_RENDERER.h"

namespace DE
{

DEBUG_RENDERER* DEBUG_RENDERER::m_pInstance = 0;

DEBUG_RENDERER::DEBUG_RENDERER()
{
	m_m2DProjection = Matrix4::OrthographicProjection(1024, 768, 0.0f, 1.0f);

	m_3DRenderPass.SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	m_3DRenderPass.SetPixelShader("../DEngine/Shaders/PS_red.hlsl");
	m_3DRenderPass.SetBlendState(State::NULL_STATE);
	m_3DRenderPass.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_3DRenderPass.SetDepthStencilState(State::NULL_STATE);
	m_3DRenderPass.SetRasterizerState(State::WIREFRAME_RS);
	m_3DRenderPass.SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
}

};