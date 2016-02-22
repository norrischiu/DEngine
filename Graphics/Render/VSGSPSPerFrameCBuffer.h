#ifndef VSGSPS_PER_FRAME_CBUFFER_H_
#define VSGSPS_PER_FRAME_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

class VSGSPSPerFrameCBuffer : public CBuffer
{

public:

	struct VSGSPS_PER_FRAME_CBUFFER
	{
		float			sysTime;
	};

	VSGSPSPerFrameCBuffer();

	void Update()
	{
		CBuffer::Update(sizeVS, sizeGS, sizePS);
	}

	static const size_t						sizeVS;
	static const size_t						sizeGS;
	static const size_t						sizePS;
};

#endif // !VSGSPS_PER_FRAME_CBUFFER_H_
