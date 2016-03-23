cbuffer CONSTANT_BUFFER
{
	float4x4 gViewProj;
	float4x4 gView;
	float4	gWorldFrustumPlanes[6];
	float4	gEyePosW;
	float	gTexelCellSpaceU;
	float	gTexelCellSpacev;
};


struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

// output
struct DS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

Texture2D shaderTexture[3];
SamplerState SampleType;

// function
[domain("quad")]
DS_OUTPUT DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HS_OUTPUT, 4> quad)
{
	DS_OUTPUT OUT;

	OUT.vPos = lerp(
		lerp(quad[0].vPos, quad[1].vPos, uv.x),
		lerp(quad[2].vPos, quad[3].vPos, uv.x),
		uv.y);
	
	OUT.vTex = lerp(
		lerp(quad[0].vTex, quad[1].vTex, uv.x),
		lerp(quad[2].vTex, quad[3].vTex, uv.x),
		uv.y);

	OUT.vNormal = quad[0].vNormal;
	OUT.vTangent = quad[0].vTangent;
	OUT.vBinormal = quad[0].vBinormal;
	OUT.vBoundsY = quad[0].vBoundsY;

	OUT.vPos.y = shaderTexture[2].SampleLevel(SampleType, OUT.vTex, 0).r * 30.0f;
	OUT.vPos = mul(OUT.vPos, gViewProj);

	return OUT;
}