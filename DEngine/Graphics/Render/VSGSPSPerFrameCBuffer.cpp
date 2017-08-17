#include "VSGSPSPerFrameCBuffer.h"
#include "Graphics\D3D12Renderer.h"
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
#ifdef D3D12
	// set the root descriptor table 0 to the constant buffer descriptor heap
//	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->SetGraphicsRootDescriptorTable(0, ((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart());

#elif defined D3D11
	// Set the same cbuffer for all shaders
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->VSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->GSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->PSSetConstantBuffers(m_iSlotID, 1, &m_Memory._buffer);
#endif
}

};