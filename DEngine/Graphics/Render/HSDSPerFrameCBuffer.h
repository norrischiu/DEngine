#ifndef HSDS_PER_FRAME_CBUFFER_H_
#define HSDS_PER_FRAME_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

namespace DE
{

	class HSDSPerFrameCBuffer : public CBuffer
	{

	public:

		struct HSDS_CBUFFER
		{
			Matrix4 gViewProj;
			Matrix4 gView;
			Vector3 gWorldFrustumPlanes[6];
			Vector3 gEyePosW;
			float	gTexelCellSpaceU;
			float	gTexelCellSpacev;
		};

		HSDSPerFrameCBuffer();

		void BindToRenderer() override;

		void Update()
		{
			CBuffer::Update(size);
		};

		static const size_t						size;
	};

};
#endif // !HSDS_PER_FRAME_CBUFFER_H_
