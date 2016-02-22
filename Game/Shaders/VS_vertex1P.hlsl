// gbuffer
cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix			WorldTransform;
	matrix			WVPTransform;
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
};

// function
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.vPos = mul(IN.vPos, WVPTransform);
	 
    return OUT;
}
