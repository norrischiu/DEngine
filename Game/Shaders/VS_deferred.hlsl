// cbuffer
cbuffer VS_CONSTANT_BUFFER
{
	matrix Transform;
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
	float2 vTex : TEXCOORD;
};

// output
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = mul(IN.vPos, Transform);
	OUT.vTex = IN.vTex;

	return OUT;
}
