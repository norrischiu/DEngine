Texture2D shaderTexture[3];
SamplerState SampleType;

// cbuffer
struct PointLight
{
	float4 vPos;
	float4 vAmbient;
	float4 vDiffuse;
	float4 vSpecular;
	float fradius;
	float fIntensity;
};

cbuffer PS_CONSTANT_BUFFER
{
	PointLight testLight;
	Matrix mProjectionInverse;
};

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	//float2 vTex : TEXCOORD0;
};

// function
float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 rgb, normal;
	float2 texCoord;
	texCoord.x = IN.vPos.x / 1024.0f;
	texCoord.y = IN.vPos.y / 768.0f;
	rgb = shaderTexture[0].Sample(SampleType, texCoord);
	normal = shaderTexture[1].Sample(SampleType, texCoord); // view space normal
	float depth = shaderTexture[2].Sample(SampleType, texCoord).r;

	// calculate view space position
	float x = texCoord.x * 2.0f - 1.0f;
	float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
	float4 projectedPos = float4(x, y, depth, 1.0f);
	float4 posVS = mul(projectedPos, mProjectionInverse);
	posVS.xyz /= posVS.w;

	// calculate lighting in view space
	float4 ambient = testLight.vAmbient;
	float4 L = testLight.vPos - posVS;
	float len = length(L.xyz) * length(L.xyz);
	float NdotL = saturate(dot(normal.xyz, normalize(L).xyz));
	float4 diffuse = saturate(testLight.vDiffuse * NdotL);
	float4 R = normalize(2.0f * NdotL * normal - L);
	float4 specular = pow(saturate(dot(R, L)), 1);
	float attenuation = 1.0f / len * 10.0f;
	
	return ambient + rgb * diffuse * attenuation;
}
