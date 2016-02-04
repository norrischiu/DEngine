Texture2D shaderTexture;

// input
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
};

static const int2 offsets[7] = { { -4, 0 },{ -3, 0 },{ -2, 0 },{ 0, 0 },{ 2, 0 },{ 3, 0 },{ 4, 0 } };
static const float weights[7] = { 0.01f, 0.28f, 0.233f, 0.474f, 0.233f, 0.28f, 0.01f };

// function
float4 PS(VS_OUTPUT IN) : SV_TARGET
{
	float2 texCoord;
	texCoord.x = IN.vPos.x;
	texCoord.y = IN.vPos.y;
	
	int3 loadPos = int3(texCoord.xy, 0);

	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (uint i = 0u; i < 7u; ++i)
	{
		//color += shaderTexture[3].Load(loadPos, offsets[i]) * weights[i];
		color += shaderTexture.Load(int3(loadPos.xy + offsets[i], 0)) * weights[i];
	}
	return color;
}
