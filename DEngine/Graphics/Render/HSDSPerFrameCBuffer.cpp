#include "HSDSPerFrameCBuffer.h"

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
	CBuffer::BindToRenderer();
	// TODO: implement this in D3D12
}

};