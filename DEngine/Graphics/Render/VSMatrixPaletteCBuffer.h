#ifndef VS_MATRIX_PALATTE_H_
#define VS_MATRIX_PALATTE_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

namespace DE
{

class VSMatrixPaletteCBuffer : public CBuffer
{

public:

	struct VS_MATRIX_PALETTE_CBUFFER
	{
		Matrix4			mSkinning[256];
	};

	VSMatrixPaletteCBuffer();

	void Update()
	{
		CBuffer::Update(size);
	}

	static const size_t						size;
};

};
#endif // !VS_MATRIX_PALATTE_H_

