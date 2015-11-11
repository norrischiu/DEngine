// Globals
cbuffer VS_CONSTANT_BUFFER
{
	matrix WorldTransform;
	matrix Transform;
};

// For now, vertex shader input is only the position
struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 vNormal : NORMAL;
	float2 vTex : TEXCOORD;
};

// For now, our vertex shader only needs to ouput
// the correct transformed position.
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : COLOR;
	float2 vTex : TEXCOORD0;
};

// Our basic vertex shader takes the model space
// position, and transforms it into screen space.
VS_OUTPUT VS(VS_INPUT IN)
{
	// Clear the output structure.
	VS_OUTPUT OUT;

	// Apply world transform, then view/projection
	OUT.vPos = mul(IN.vPos, Transform);
	OUT.vNormal = mul(IN.vNormal, WorldTransform);
	OUT.vTex = IN.vTex;

	// Done--return the output.
	return OUT;
}
