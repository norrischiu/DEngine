Texture2D shaderTexture;

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
};

// function
float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	return shaderTexture.Load(int3(IN.vPos.xy, 0)) * 0.2f;
}
