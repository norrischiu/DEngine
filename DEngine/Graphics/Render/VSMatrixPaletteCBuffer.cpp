#include "VSMatrixPaletteCBuffer.h"

namespace DE
{

const size_t VSMatrixPaletteCBuffer::size = sizeof(VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER);

VSMatrixPaletteCBuffer::VSMatrixPaletteCBuffer()
	: CBuffer(type::VertexShader, size)
{
	m_iSlotID = 2;
}

};