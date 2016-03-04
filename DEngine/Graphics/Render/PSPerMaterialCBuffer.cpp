#include "PSPerMaterialCBuffer.h"

namespace DE
{

const size_t PSPerMaterialCBuffer::size = sizeof(PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER);

PSPerMaterialCBuffer::PSPerMaterialCBuffer()
	: CBuffer(type::PixelShader, size)
{
	m_iSlotID = 1;
}

};