Texture2D shaderTexture;
SamplerState SampleType;

struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 color;
	color = shaderTexture.Sample(SampleType, IN.vTex);
    return color;
}
