// cbuffer
cbuffer VS_PER_OBJECT : register(b1)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

cbuffer VS_MATRIX_PALETTE : register(b2)
{
	matrix		mSkinning[256];
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	int4 vJointIndex : BONEINDICES;
	float4 vSkinWeight : WEIGHTS;
	float2 vTex : TEXCOORD;
};

// output
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	OUT.vPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
	OUT.vNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	OUT.vTangent = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		OUT.vPos += IN.vSkinWeight[i] * mul(float4(IN.vPos.xyz, 1.0f), mSkinning[IN.vJointIndex[i]]);
		OUT.vNormal += IN.vSkinWeight[i] * mul(IN.vNormal.xyz, mSkinning[IN.vJointIndex[i]]);
		OUT.vTangent += IN.vSkinWeight[i] * mul(IN.vTangent.xyz, mSkinning[IN.vJointIndex[i]]);
	}
	OUT.vPos = mul(float4(OUT.vPos.xyz, 1.0f), WVPTransform);
	OUT.vNormal = mul(OUT.vNormal, WorldTransform);
	OUT.vTangent = mul(OUT.vTangent, WorldTransform);
	float3 binormal = normalize(cross(IN.vNormal.xyz, IN.vTangent.xyz));
	OUT.vBinormal = mul(float4(binormal.xyz, 0.0f), WorldTransform);
	OUT.vTex = IN.vTex;

	return OUT;
}
