#include "VSMatrixPaletteCBuffer.h"

const size_t VSMatrixPaletteCBuffer::sizeVS = sizeof(VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER);

VSMatrixPaletteCBuffer::VSMatrixPaletteCBuffer()
	: CBuffer(type::VS_ONLY, sizeVS, NULL)
{
	m_iSlotID = 1;
}
