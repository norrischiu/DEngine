Texture2D shaderTexture[2];
SamplerState SampleType;

// Globals
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
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
};

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 color, bump, normal;
	color = shaderTexture[0].Sample(SampleType, IN.vTex);
	bump = (shaderTexture[1].Sample(SampleType, IN.vTex) * 2.0f) - 1.0f;
	normal = normalize((bump.x * IN.vTangent) + (bump.y * IN.vBinormal) + (bump.z * IN.vNormal));

	float4 L = IN.vPos - testLight.vPos;
	float4 NdotL = saturate(dot(L, normal));
	float4 diffuse = testLight.vDiffuse * NdotL;

	float4 R = normalize(2.0f * NdotL * normal - L);
	float4 specular = pow(saturate(dot(R, L)), 1);

	return color * (testLight.vAmbient + diffuse + specular);
}
