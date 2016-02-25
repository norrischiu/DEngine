// cbuffer
cbuffer VS_PER_OBJECT : register(b1)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

// Input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float2 vTex : TEXCOORD;
};

// Output
struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

// Function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = mul(IN.vPos, WVPTransform);
	OUT.vTex = IN.vTex;
	 
    return OUT;
}
