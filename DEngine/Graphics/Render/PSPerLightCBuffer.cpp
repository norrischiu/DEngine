#include "PSPerLightCBuffer.h"

namespace DE
{

const size_t PSPerLightCBuffer::size = sizeof(PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER);

PSPerLightCBuffer::PSPerLightCBuffer()
	: CBuffer(type::PixelShader, size)
{
	m_iSlotID = 2;
}

};