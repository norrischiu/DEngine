Texture2D shaderTexture;
SamplerState SampleType;

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : COLOR;
	float2 vTex : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

PS_OUTPUT PS(VS_OUTPUT IN) : SV_TARGET
{
	PS_OUTPUT OUT;
	OUT.color = shaderTexture.Sample(SampleType, IN.vTex);
	OUT.normal = float4(IN.vNormal, 1.0f);
	return OUT;
}
