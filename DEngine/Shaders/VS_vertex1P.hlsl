// gbuffer
cbuffer VS_PER_OBJECT : register(b1)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
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

	OUT.vPos = mul(float4(IN.vPos.xyz, 1.0f), WVPTransform);
	 
    return OUT;
}
