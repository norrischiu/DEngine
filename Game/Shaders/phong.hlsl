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
	float4 vNormal : COLOR;
	float2 vTex : TEXCOORD0;
};

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 color1, color2;
	color1 = shaderTexture[0].Sample(SampleType, IN.vTex);
	//color2 = shaderTexture[1].Sample(SampleType, IN.vTex);
	float4 L = IN.vPos - testLight.vPos;
	float4 NdotL = saturate(dot(L, IN.vNormal));
	float4 diffuse = testLight.vDiffuse * NdotL;

	float4 R = normalize(2 * NdotL * IN.vNormal - L);
	float4 specular = pow(saturate(dot(R, L)), 1);

	return color1 * (testLight.vAmbient + diffuse + specular);
}
