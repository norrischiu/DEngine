#include "VSPerObjectCBuffer.h"
#include "D3D11Renderer.h"

const size_t VSPerObjectCBuffer::sizeVS = sizeof(VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER);

VSPerObjectCBuffer::VSPerObjectCBuffer()
	: CBuffer(type::VS_ONLY, sizeVS, NULL)
{
	m_iSlotID = 0;
}