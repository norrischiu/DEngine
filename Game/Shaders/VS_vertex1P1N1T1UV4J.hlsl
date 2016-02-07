// cbuffer
cbuffer VS_PER_OBJECT : register(b0)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

cbuffer VS_MATRIX_PALETTE : register(b1)
{
	matrix		mSkinning[256];
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float2 vTex : TEXCOORD;
	int4 vJointIndex : BONEINDICES;
	float4 vSkinWeight : WEIGHTS;
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

	for (int i = 0; i < 4; ++i)
	{
		OUT.vPos = vSkinWeight[i] * mul(IN.vPos, m_mSkinning[i]);
		OUT.vNormal = vSkinWeight[i] * mul(IN.vNormal, m_mSkinning[i]);
		OUT.vTangent = vSkinWeight[i] * mul(IN.vTangent, m_mSkinning[i]);
	}
	OUT.vPos = mul(OUT.vPos, WVPTransform);
	OUT.vNormal = mul(OUT.vNormal, WorldTransform);
	OUT.vTangent = mul(OUT.vTangent, WorldTransform);
	float3 binormal = normalize(cross(IN.vNormal.xyz, IN.vTangent.xyz));
	OUT.vBinormal = mul(float4(binormal, 0), WorldTransform);
	OUT.vTex = IN.vTex;

	return OUT;
}
