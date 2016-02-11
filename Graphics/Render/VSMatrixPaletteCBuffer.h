#ifndef VS_MATRIX_PALATTE_H_
#define VS_MATRIX_PALATTE_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

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
		CBuffer::Update(sizeVS, NULL);
	}

	static const size_t						sizeVS;
	static const size_t						sizePS;
};

#endif // !VS_MATRIX_PALATTE_H_

