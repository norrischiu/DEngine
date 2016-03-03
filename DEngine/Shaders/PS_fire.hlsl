Texture2D shaderTexture;
#define cMult 0.0001002707309736288
#define aSubtract 0.2727272727272727

// globals
cbuffer CB_PER_FRAME : register(b0)
{
	float4x4	gViewProj;
	float4x4	gClipToView;
	float4		gEyePosW;

	float4		gEmitPosW;
	float4		gEmitDirW;

	float		gTimeStep;
	float		gFlareAge;
	unsigned int gMaxParts;
};

struct GS_OUTPUT
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
};



// Draw
float4 PS(GS_OUTPUT pin) : SV_TARGET
{

	float2 texCoord;
	texCoord.x = pin.PosH.x / 1024.0f;
	texCoord.y = pin.PosH.y / 768.0f;

	float4 rgb = shaderTexture.Load(int3(pin.PosH.xy, 0));

	float x = texCoord.x * 2.0f - 1.0f;
	float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
	float4 projectedPos = float4(x, y, 1.0f, 1.0f);
	float4 posVS = mul(projectedPos, gClipToView);
	posVS.xyz /= posVS.w;


	return pin.Color;
}
