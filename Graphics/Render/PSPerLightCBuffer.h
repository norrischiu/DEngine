#ifndef PS_PER_LIGHT_CBUFFER_H_
#define PS_PER_LIGHT_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

class PSPerLightCBuffer : public CBuffer
{

public:

	struct LightGPU
	{
		Matrix4		mWorldToLightClip;
		Matrix4		mLightClipToView;
		Vector3		vPos;
		Vector3		vDir;
		Vector4		vColor;
		float		fIntensity;
		float		fInnerAngle;
		float		fOuterAngle;
		float		fRadius;
		int			iType;
		int			bIsCastShadow;
	};

	struct PS_PER_LIGHT_CBUFFER
	{
		LightGPU	light;
		Matrix4		mClipToView; // to compute view space position from screen space
		Matrix4		mViewToClip;
		Matrix4		mViewToWorld;
	};

	PSPerLightCBuffer();

	void Update()
	{
		CBuffer::Update(NULL, sizePS);
	}

	static const size_t						sizeVS;
	static const size_t						sizePS;
};


#endif // !PS_PER_LIGHT_CBUFFER_H_