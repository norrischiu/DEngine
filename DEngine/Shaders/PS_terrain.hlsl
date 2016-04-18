Texture2D shaderTexture[3];
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
}

cbuffer CONSTANT_BUFFER : register(b3)
{
	float4x4 gViewProj;
	float4	gWorldFrustumPlanes[6];
	float4	gEyePosW;
	float	gTexelCellSpaceU;
	float	gTexelCellSpaceV;
};

// input
struct DS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

// output
struct PS_OUTPUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

// function
PS_OUTPUT PS(DS_OUTPUT IN) : SV_TARGET
{
	PS_OUTPUT OUT;
	OUT.color = shaderTexture[0].Sample(SampleType, IN.vTex);

	float2 left = IN.vTex + float2(-gTexelCellSpaceU, 0.0f);
	float2 right = IN.vTex + float2(gTexelCellSpaceU, 0.0f);
	float2 bottom = IN.vTex + float2(0.0f, gTexelCellSpaceU);
	float2 top = IN.vTex + float2(0.0f, -gTexelCellSpaceU);


	float leftY = shaderTexture[2].SampleLevel(SampleType, left, 0).r;
	float rightY = shaderTexture[2].SampleLevel(SampleType, right, 0).r;
	float bottomY = shaderTexture[2].SampleLevel(SampleType, bottom, 0).r;
	float topY = shaderTexture[2].SampleLevel(SampleType, top, 0).r;


	/*float leftY = shaderTexture[2].Load(int3(left, 0)).r;
	float rightY = shaderTexture[2].Load(int3(right, 0)).r;
	float bottomY = shaderTexture[2].Load(int3(bottom, 0)).r;
	float topY = shaderTexture[2].Load(int3(top, 0)).r;*/

	float3 tangent = normalize(float3(1.0f, rightY - leftY, 0.0f));
	float3 bitan = normalize(float3(0.0f, bottomY - topY, -1.0f));
	float3 normal = cross(tangent, bitan);

	float4 bump = (shaderTexture[1].Sample(SampleType, IN.vTex) * 2.0f) - float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3x3 TBN =
	{
		tangent.x, tangent.y, tangent.z,
		bitan.x, bitan.y, bitan.z,
		normal.x, normal.y, normal.z
	};
	//OUT.normal = normalize(float4(mul(bump.xyz, TBN), 0));

	OUT.normal = mul(gViewProj, float4(normal, 1.0f));

	// pack specular factor and shininess into gbuffer
	OUT.color.a = material.vSpecular.x;
	OUT.normal.a = material.vShininess;

	return OUT;
}

