#include "PSPerMaterialCBuffer.h"

const size_t PSPerMaterialCBuffer::sizePS = sizeof(PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER);

PSPerMaterialCBuffer::PSPerMaterialCBuffer()
	: CBuffer(type::PS_ONLY, NULL, sizePS)
{
	m_iSlotID = 0;
}
