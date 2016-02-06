// cbuffer
cbuffer VS_PER_OBJECT : register(b0)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

cbuffer VS_MATRIX_PALETTE : register(b1)
{
	matrix		m_mSkinning[256];
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float2 vTex : TEXCOORD;
	int4 vJointIndex : BONEINDICES;
	float4 vJointWeight : WEIGHTS;
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

	OUT.vPos = mul(IN.vPos, WVPTransform);
	OUT.vNormal = mul(IN.vNormal, WorldTransform);
	OUT.vTangent = mul(IN.vTangent, WorldTransform);
	float3 binormal = normalize(cross(IN.vNormal.xyz, IN.vTangent.xyz));
	OUT.vBinormal = mul(float4(binormal, 0), WorldTransform);
	OUT.vTex = IN.vTex;

	return OUT;
}
