Texture2D shaderTexture[3];

// cbuffer

static matrix mClipToTex =
	{ 0.5,	 0,		0,		0,
	0,		-0.5,	0,		0,
	0,		0,		1,		0,
	0.5,	0.5,	0.0,	1.0 };

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
};

// function
float linearizeZ(float z)
{
	return mViewToClip._43 / (z - mViewToClip._33);
}

float3 TextureSpaceToViewSpace(float2 posTS, float depth)
{
	float x = posTS.x * 2.0f - 1.0f;
	float y = (1.0f - posTS.y) * 2.0f - 1.0f;
	float4 projectedPos = float4(x, y, depth, 1.0f);
	float4 posVS = mul(projectedPos, mClipToView);
	posVS.xyz /= posVS.w;
	return posVS.xyz;
}

#define tolerance 0.01f

bool intersectDepthBuffer(float z, float minZ, float maxZ)
{
	if (z - tolerance > maxZ || z + tolerance < minZ)
		return false;
	else
		return true;
}

#define VSMaxDist 8.0f
#define MaxStep 30.0f
#define PixelStride 16.0f

bool RayMarch(float3 originVS, float3 rayVS, inout float2 hitPixel, float jitter)
{
	float z = 0.0f, depth = 100.0f;
	float3 endptVS = originVS + rayVS * VSMaxDist;

	mClipToTex = mul(mViewToClip, mClipToTex);

	float4 H0 = mul(float4(originVS, 1.0f), mClipToTex);
	H0.xy *= float2(1024.0f, 768.0f);
	float4 H1 = mul(float4(endptVS, 1.0f), mClipToTex);
	H1.xy *= float2(1024.0f, 768.0f);

	float k0 = 1.0f / H0.w;
	float k1 = 1.0f / H1.w;

	float3 Q0 = originVS * k0;
	float3 Q1 = endptVS * k1; // <--?

	float2 P0 = H0.xy * k0;
	float2 P1 = H1.xy * k1;

	float2 delta = P1 - P0;

	bool permute = false;
	if (abs(delta.x) < abs(delta.y))
	{
		// This is a more-vertical line
		permute = true;
		delta = delta.yx;
		P0 = P0.yx;
		P1 = P1.yx;
	}

	float stepDir = sign(delta.x);
	float invdx = stepDir / delta.x;

	// Track the derivatives of Q and k
	float3 dQ = (Q1 - Q0) * invdx; // <--?
	float dk = (k1 - k0) * invdx;
	float2 dP = float2(stepDir, delta.y * invdx);

	dP *= PixelStride;
	dQ *= PixelStride;
	dk *= PixelStride;

	// set initial value
	P0 += dP;
	Q0 += dQ;
	k0 += dk;

	float4 PQk = float4(P0, Q0.z, k0);
	float4 dPQk = float4(dP, dQ.z, dk);
	float3 Q = Q0;

	float prevZMaxEstimate = originVS.z;
	float rayZMin = prevZMaxEstimate;
	float rayZMax = prevZMaxEstimate;

	for (int step = 0; (step < MaxStep) && !intersectDepthBuffer(depth, rayZMin, rayZMax); step++)
	{
		rayZMin = prevZMaxEstimate;
		rayZMax = (dPQk.z * 0.5f + PQk.z) / (dPQk.w * 0.5f + PQk.w);
		prevZMaxEstimate = rayZMax;
		if (rayZMin > rayZMax)
		{
			float temp = rayZMin;
			rayZMin = rayZMax;
			rayZMax = temp;
		}

		hitPixel = permute ? PQk.yx : PQk.xy;
		depth = linearizeZ(shaderTexture[2].Load(int3(hitPixel.xy, 0)).r);

		PQk += dPQk;
	}

	return intersectDepthBuffer(depth, rayZMin, rayZMax);
}

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float3 normalVS;
	float2 texCoord;
	texCoord.x = IN.vPos.x / 1024.0f;
	texCoord.y = IN.vPos.y / 768.0f;
	normalVS = shaderTexture[1].Load(int3(IN.vPos.xy, 0)).xyz;
	float depth = shaderTexture[2].Load(int3(IN.vPos.xy, 0)).r;

	// calculate view space position
	float x = texCoord.x * 2.0f - 1.0f;
	float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
	float4 projectedPos = float4(x, y, depth, 1.0f);
	float4 posVS = mul(projectedPos, mClipToView);
	posVS.xyz /= posVS.w;

	float2 hitPixel = float2(0.0f, 0.0f);
	float3 rayToPosVS = normalize(posVS.xyz);
	float3 rayVS = normalize(reflect(rayToPosVS, normalVS));
	float rDotV = saturate(dot(rayVS, rayToPosVS));
	float jitter = PixelStride > 1.0f ? float(int(IN.vPos.x + IN.vPos.y) & 1) * 0.5f : 0.0f;
	bool hasIntersected = RayMarch(posVS.xyz, rayVS, hitPixel, jitter);
	float3 rgb = shaderTexture[0].Load(int3(hitPixel, 0)).rgb;

	return float4(rgb, rDotV) * (hasIntersected ? 1.0f : 0.0f);
}
