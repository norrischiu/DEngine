Texture2D shaderTexture[2];
SamplerState SampleType;

// cbuffer
struct PointLight
{
	float4 vPos;
	float4 vAmbient;
	float4 vDiffuse;
	float4 vSpecular;
	float fRadius;
	float fIntensity;
};
cbuffer PS_CONSTANT_BUFFER
{
	PointLight testLight;
};


struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 color;
	color = shaderTexture[0].Sample(SampleType, IN.vTex);
	return color;
}
