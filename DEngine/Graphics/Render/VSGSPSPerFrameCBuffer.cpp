#include "VSGSPSPerFrameCBuffer.h"
#include "Graphics\D3D11Renderer.h"

namespace DE
{

const size_t VSGSPSPerFrameCBuffer::size = sizeof(VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER);

VSGSPSPerFrameCBuffer::VSGSPSPerFrameCBuffer()
	: CBuffer(type::VertexShader, size)
{
	m_iSlotID = 0;
}

void VSGSPSPerFrameCBuffer::BindToRenderer()
{
	// Set the same cbuffer for all shaders
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->GSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
}

};