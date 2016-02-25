#include "RenderPass.h"
#include "Graphics\D3D11Renderer.h"

void RenderPass::BindToRenderer()
{
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->GSSetShader(m_pGS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->RSSetState(m_pRS);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetDepthStencilState(m_pDSS, 1);
	D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetBlendState(m_pBS, NULL, 0xFFFFFF);
	if (m_pSOTarget)
	{
		const unsigned int offset = 0;
		D3D11Renderer::GetInstance()->m_pD3D11Context->SOSetTargets(1, &m_pSOTarget, &offset);
	}
	if (m_iRTVNum > 0)
	{
		D3D11Renderer::GetInstance()->UnbindRenderTargets();
		D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetRenderTargets(m_iRTVNum, m_pRTVs, m_pDSV);
	}
	else
	{
		D3D11Renderer::GetInstance()->UnbindRenderTargets();
		D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetRenderTargets(0, nullptr, m_pDSV);
	}
	for (int i = 0; i < m_vTextureSRVs.size(); i++)
	{
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(i, 1, &m_vTextureSRVs[i]);
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_vSamplerState[i]);
	}
}
