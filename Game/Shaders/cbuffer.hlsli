
cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

struct Light
{
	float4		vPosVS;
	float4		vDirVS;
	float4		vColor;
	float		fIntensity;
	float		fInnerAngle;
	float		fOuterAngle;
	float		fRadius;
	int			iType;
};

cbuffer PS_CONSTANT_BUFFER : register(b1)
{
	Light		light;
	Matrix		mClipToView;
	Matrix		mViewToClip;
};