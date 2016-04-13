// cbuffer
cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix		WorldTransform;
	matrix		WVPTransform;
};

struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float4 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vColor : COLOR;
};

VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	
	OUT.vPos = mul(IN.vPos, WVPTransform);
	OUT.vNormal = IN.vNormal;
	OUT.vColor = IN.vColor;
	 
    return OUT;
}