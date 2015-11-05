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

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

// function
float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 rgb, normal;
	rgb = shaderTexture[0].Sample(SampleType, IN.vTex);
	normal = shaderTexture[1].Sample(SampleType, IN.vTex);
	float4 L = IN.vPos - testLight.vPos;
	float4 NdotL = saturate(dot(L, normal));
	float4 diffuse = testLight.vDiffuse * NdotL;

	float4 R = normalize(2 * NdotL * normal - L);
	float4 specular = pow(saturate(dot(R, L)), 1);

	return rgb * (testLight.vAmbient + diffuse + specular);
}
