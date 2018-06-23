Texture2D shaderTexture[2];
SamplerState SampleType;

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
};

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
};

// output
struct PS_OUTPUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

// function
PS_OUTPUT PS(VS_OUTPUT IN) : SV_TARGET
{
	PS_OUTPUT OUT;
	OUT.color = shaderTexture[0].Sample(SampleType, IN.vTex);
	float4 bump = (shaderTexture[1].Sample(SampleType, IN.vTex) * 2.0f) - float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3x3 TBN =
	{
		IN.vTangent.x, IN.vTangent.y, IN.vTangent.z,
		IN.vBinormal.x, IN.vBinormal.y, IN.vBinormal.z,
		IN.vNormal.x, IN.vNormal.y, IN.vNormal.z
	};
	OUT.normal = normalize(float4(mul(bump.xyz, TBN), 0));

	// pack specular factor and shininess into gbuffer
	OUT.color.a = material.vSpecular;
	OUT.normal.a = material.vShininess;

return OUT;
}
