struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
	float4 vColor : COLOR;
};

float4 PS(VS_OUTPUT IN) : SV_TARGET
{
    return IN.vColor;
}
