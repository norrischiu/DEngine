#include "PSPerLightCBuffer.h"

const size_t PSPerLightCBuffer::sizePS = sizeof(PSPerLightCBuffer::PS_PER_LIGHT_CBUFFER);

PSPerLightCBuffer::PSPerLightCBuffer()
	: CBuffer(type::PS_ONLY, NULL, NULL, sizePS)
{
	m_iSlotID = 2;
}
