
/*
* PIXEL SHADER
*/
// globals
cbuffer CB_PER_FRAME : register(b0)
{
	float4x4	gViewProj;
	float4x4	gClipToView;
	float4		gEyePosW;

	float4		gEmitPosW;
	float4		gEmitDirW;

	float		gTimeStep;
	float		gFlareAge;
	float		gSize;
	unsigned int gMaxParts;
	unsigned int gEffectType;
};

// cbuffer
struct Material
{
	float4		vEmissive;
	float4		vDiffuse;
	float4		vSpecular;
	float		vShininess;
};

cbuffer PS_CONSTANT_BUFFER : register(b1)
{
	Material	material;
}

struct Light
{
	matrix		mWorldToLightClip;
	matrix		mLightClipToView;
	float4		vPosVS;
	float4		vDirVS;
	float4		vColor;
	float		fIntensity;
	float		fInnerAngle;
	float		fOuterAngle;
	float		fRadius;
	int			iType;
	bool		bIsCastShadow;
};

cbuffer PS_CONSTANT_BUFFER : register(b2)
{
	Light		light;
	Matrix		mClipToView;
	Matrix		mViewToClip;
	Matrix		mViewToWorld;
};

cbuffer CONSTANT_BUFFER : register(b3)
{
	float4x4 gViewProj;
	float4	gWorldFrustumPlanes[6];
	float3	gEyePosW;
	float	gTexelCellSpaceU;
	float	gTexelCellSpacev;
};