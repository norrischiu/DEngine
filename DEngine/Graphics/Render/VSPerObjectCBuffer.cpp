#include "VSPerObjectCBuffer.h"

namespace DE
{

const size_t VSPerObjectCBuffer::size = sizeof(VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER);

VSPerObjectCBuffer::VSPerObjectCBuffer()
	: CBuffer(type::VertexShader, size)
{
	m_iSlotID = 1;
}

};