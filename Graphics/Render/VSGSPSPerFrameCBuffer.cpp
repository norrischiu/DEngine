#include "VSGSPSPerFrameCBuffer.h"

const size_t VSGSPSPerFrameCBuffer::sizeVS = sizeof(VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER);
const size_t VSGSPSPerFrameCBuffer::sizeGS = sizeof(VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER);
const size_t VSGSPSPerFrameCBuffer::sizePS = sizeof(VSGSPSPerFrameCBuffer::VSGSPS_PER_FRAME_CBUFFER);

VSGSPSPerFrameCBuffer::VSGSPSPerFrameCBuffer()
	: CBuffer(type::VS_GS_PS, sizeVS, sizeGS, sizePS)
{
	m_iSlotID = 0;
}