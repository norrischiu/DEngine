#include "VSGSPSPerFrameCBuffer.h"
#include "Graphics\D3D12Renderer.h"

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
	// TODO: implement this in D3D12
	// set the root descriptor table 0 to the constant buffer descriptor heap
//	Renderer::GetInstance()->m_pCommandList->SetGraphicsRootDescriptorTable(0, Renderer::GetInstance()->m_pCbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart());
}

};