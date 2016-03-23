Texture2D shaderTexture[4];

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// cbuffer
struct Material
{
	float		fSpecular;
	float		fShininess;
};

struct Light
{
	matrix		mWorldToLightClip;
	matrix		mLightClipToView;
	float4		vPosVS;
	float4		vDirVS;
	float4		vColor;
	float		fIntensity;
	float		fInnerAngle;
	float		fOuterAngle;
	float		fRadius;
	int			iType;
	bool		bIsCastShadow;
};

cbuffer PS_CONSTANT_BUFFER : register(b2)
{
	Light		light;
	Matrix		mClipToView;
	Matrix		mViewToClip;
	Matrix		mViewToWorld;
};

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	//float2 vTex : TEXCOORD0;
};

// function
float4 PointLightPass(Material mat, float4 rgb, float3 L, float3 N, float V)
{
	float len = length(L);
	//float attenuation = 1.0f / (len * len);
	float attenuation = 1.0f - smoothstep(0.0f, light.fRadius, len);
	float NdotL = max(dot(N, normalize(L)), 0);
	float4 diffuse = saturate(light.vColor * NdotL) * light.fIntensity;

	float3 H = normalize(L + V);
	float RdotV = max(dot(H, N), 0);
	float4 specular = light.vColor * mat.fSpecular * pow(RdotV, mat.fShininess) * light.fIntensity;

	return rgb * (diffuse + specular) * attenuation;
	//return (diffuse + specular) * attenuation;
}

float4 SpotLightPass(Material mat, float4 rgb, float3 L, float3 N, float V)
{
	float len = length(L);
	//float attenuation = 1.0f / (len * len);
	float attenuation = 1.0f - smoothstep(0.0f, light.fRadius, len);
	float NdotL = max(dot(N, normalize(L)), 0);
	float4 diffuse = saturate(light.vColor * NdotL) * light.fIntensity;

	float3 H = normalize(L + V);
	float RdotV = max(dot(H, N), 0);
	float4 specular = light.vColor * mat.fSpecular * pow(RdotV, mat.fShininess) * light.fIntensity;

	float cosAngle = dot(light.vDirVS.xyz, -normalize(L));
	return rgb * (diffuse + specular) * attenuation * smoothstep(cos(light.fOuterAngle), cos(light.fInnerAngle), cosAngle);
	//return (diffuse + specular) * attenuation * smoothstep(cos(light.fOuterAngle), cos(light.fInnerAngle), cosAngle);
}

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float4 rgb, normalVS;
	float2 texCoord;
	texCoord.x = IN.vPos.x / 1024.0f;
	texCoord.y = IN.vPos.y / 768.0f;
	rgb = shaderTexture[0].Load(int3(IN.vPos.xy, 0));
	return rgb;//
	normalVS = shaderTexture[1].Load(int3(IN.vPos.xy, 0));
	float depth = shaderTexture[2].Load(int3(IN.vPos.xy, 0)).r;
	Material mat;
	mat.fSpecular = rgb.a;
	mat.fShininess = normalVS.a;

	// calculate view space position
	float x = texCoord.x * 2.0f - 1.0f;
	float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
	float4 projectedPos = float4(x, y, depth, 1.0f);
	float4 posVS = mul(projectedPos, mClipToView);
	posVS.xyz /= posVS.w;

	// calculate lighting in view space
	float4 L = light.vPosVS - posVS;

	float isShadow = 1.0;
	if (light.bIsCastShadow)
	{
		float4 posWS = mul(float4(posVS.xyz, 1.0f), mViewToWorld);
		float4 posSMCS = mul(posWS, light.mWorldToLightClip);
		posSMCS.xyz /= posSMCS.w;
		float2 shadowPixel;
		if (posSMCS.x > 1.0f || posSMCS.x < -1.0f || posSMCS.y > 1.0f || posSMCS.y < -1.0f)
		{
			isShadow = 1.0f;
		}
		else
		{
			shadowPixel.x = posSMCS.x * 0.5f + 0.5f;
			shadowPixel.y = posSMCS.y * -0.5f + 0.5f;
			float2 hitPixel = shadowPixel.xy * float2(1024.0f, 768.0f);
			float shadowMapDepth = shaderTexture[3].Load(int3(hitPixel.xy, 0)).r;
			float4 posSMPS = float4(posSMCS.xy, shadowMapDepth, 1.0f);
			float4 posSMVS = mul(posSMPS, light.mLightClipToView);
			posSMVS.xyz /= posSMVS.w;
			float4 shadowMapL = light.vPosVS - posSMVS;

			// shadowing
			isShadow = (length(L.xyz) - 0.05f > length(shadowMapL.xyz) ? 0.0f : 1.0f);
		}
	}

	switch (light.iType)
	{
	case POINT_LIGHT:
		return PointLightPass(mat, rgb, L.xyz, normalVS.xyz, -posVS.xyz) * isShadow;
	case SPOT_LIGHT:
		return SpotLightPass(mat, rgb, L.xyz, normalVS.xyz, -posVS.xyz) * isShadow;
	default:
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
