#include "HSDSPerFrameCBuffer.h"
#include "Graphics\D3D11Renderer.h"



namespace DE
{

const size_t HSDSPerFrameCBuffer::size = sizeof(HSDSPerFrameCBuffer::HSDS_CBUFFER);


HSDSPerFrameCBuffer::HSDSPerFrameCBuffer()
	: CBuffer(type::HullShader, size)
{
	m_iSlotID = 0;
}

void HSDSPerFrameCBuffer::BindToRenderer()
{
	D3D11Renderer::GetInstance()->m_pD3D11Context->HSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->DSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetConstantBuffers(3, 1, &m_Memory._buffer);
}

};