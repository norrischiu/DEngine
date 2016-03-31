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
	float2 vTex : TEXCOORD;
	float2 vBoundsY : BOUNDSY;
};

// output
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = IN.vPos;
	OUT.vTex = IN.vTex;
	OUT.vBoundsY = IN.vBoundsY;

	return OUT;
}
