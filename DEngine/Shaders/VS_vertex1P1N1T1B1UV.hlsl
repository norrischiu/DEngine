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
	float2 vBoundsY : BOUNDSY;
};

// output
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = IN.vPos;
	OUT.vNormal = mul(IN.vNormal, WorldTransform);
	OUT.vTangent = mul(IN.vTangent, WorldTransform);
	float3 binormal = normalize(cross(IN.vNormal.xyz, IN.vTangent.xyz));
	OUT.vBinormal = mul(float4(binormal, 0), WorldTransform);
	OUT.vTex = IN.vTex;
	OUT.vBoundsY = IN.vBoundsY;

	return OUT;
}
