#ifndef VSGSPS_PER_FRAME_CBUFFER_H_
#define VSGSPS_PER_FRAME_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

class VSGSPSPerFrameCBuffer : public CBuffer
{

public:

	struct VSGSPS_PER_FRAME_CBUFFER
	{

		Matrix4 gViewProj;
		Vector3 gEyePosW;
		Vector3 gEmitPosW;
		Vector3 gEmitDirW;
		float	gGameTime;
		float	gTimeStep;
		unsigned int gMaxParts;
	};

	VSGSPSPerFrameCBuffer();

	void BindToRenderer() override;

	void Update()
	{
		CBuffer::Update(size);
	};

	static const size_t						size;
};

#endif // !VSGSPS_PER_FRAME_CBUFFER_H_
