// cbuffer
cbuffer VS_CONSTANT_BUFFER
{
	matrix WorldTransform;
	matrix Transform;
};

// input
struct VS_INPUT
{
	float4 vPos : POSITION;
};

// output
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	//float2 vTex : TEXCOORD0;
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = mul(IN.vPos, Transform);
	//float2 screenPos = OUT.vPos.xy / OUT.vPos.w;
	//OUT.vTex = 0.5f * (float2(screenPos.x, -screenPos.y) + 1);
	return OUT;
}
