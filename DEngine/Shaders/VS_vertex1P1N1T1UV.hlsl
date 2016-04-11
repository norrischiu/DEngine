// cbuffer
cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
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

	OUT.vPos = mul(IN.vPos, WVPTransform);
	OUT.vNormal = mul(float4(IN.vNormal.xyz, 0.0f), WorldTransform);
	OUT.vTangent = mul(float4(IN.vTangent.xyz, 0.0f), WorldTransform);
	float3 binormal = normalize(cross(IN.vNormal.xyz, IN.vTangent.xyz));
	OUT.vBinormal = mul(float4(binormal, 0), WorldTransform);
	OUT.vTex = IN.vTex;

	return OUT;
}
