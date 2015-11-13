Texture2D shaderTexture[2];
SamplerState SampleType;

// cbuffer
struct PointLight
{
	float4 vDir;
	float4 vAmbient;
	float4 vDiffuse;
	float4 vSpecular;
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

	float3 zero = { 0, 0, 0 };
	if (normal.x == zero.x)
	{
		return float4(0, 0, 1, 1);
	}

	float4 ambient = testLight.vAmbient * testLight.fIntensity;
	float4 L = -testLight.vDir;
	float NdotL = saturate(dot(normal, L));
	float4 diffuse = saturate(testLight.vDiffuse * NdotL);
	float4 R = normalize(2.0f * NdotL * normal - L);
	float4 specular = pow(saturate(dot(R, L)), 1);

	return (ambient + diffuse * 5) * rgb;
	//return rgb + testLight.vAmbient + diffuse;
}
