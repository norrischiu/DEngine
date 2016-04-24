Texture2D shaderTexture;
SamplerState SampleType;

// globals
cbuffer CB_PER_FRAME : register(b0)
{
	float4x4	gViewProj;
	float4x4	gClipToView;
	float4		gEyePosW;

	float4		gEmitPosW;
	float4		gEmitDirW;

	float		gTimeStep;
	float		gDisableTime;
	float		gFlareAge;
	float		gSize;
	unsigned int gMaxParts;
	unsigned int gEffectType;
};

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct GS_OUTPUT
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Tex   : TEXCOORD;
};

// Draw
float4 PS(GS_OUTPUT pin) : SV_TARGET
{
	float2 tex = pin.Tex;
	if (gEffectType == 2)
	{
		float sinX = sin(gFlareAge);
		float cosX = cos(gFlareAge);
		float sinY = sin(gFlareAge);
		float2x2 rotationMatrix = float2x2(cosX, -sinX, sinY, cosX);
		tex.xy = mul(pin.Tex.xy, rotationMatrix);
	}

	//return gTexArray.Sample(samLinear, float3(pin.Tex, 0))*pin.Color;

	return shaderTexture.Sample(SampleType, pin.Tex);
}
