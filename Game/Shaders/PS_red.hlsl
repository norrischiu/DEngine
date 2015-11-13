struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
};


struct PS_OUTPUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

PS_OUTPUT PS(VS_OUTPUT IN) : SV_TARGET
{
	PS_OUTPUT OUT;
	OUT.color = float4(1.0, 0.0, 0.0, 1.0);
	OUT.normal = float4(1.0, 1.0, 1.0, 1.0);
	return OUT;
}
