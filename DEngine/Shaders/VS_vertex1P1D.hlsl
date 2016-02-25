// Globals
cbuffer VS_CONSTANT_BUFFER
{
	matrix CameraMat;
	matrix ProjectionMat;
	matrix TransformationMat;
};

// For now, vertex shader input is only the position
struct VS_INPUT
{
	float4 vPos : POSITION;
	float2 vTex : TEXCOORD;
};

// For now, our vertex shader only needs to ouput
// the correct transformed position.
struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

// Our basic vertex shader takes the model space
// position, and transforms it into screen space.
VS_OUTPUT VS(VS_INPUT IN)
{
    // Clear the output structure.
	VS_OUTPUT OUT;
	
	// Apply world transform, then view/projection
	//OUT.vPos = mul(CameraMat, IN.vPos);
	OUT.vPos = mul(IN.vPos, TransformationMat);
	OUT.vPos = mul(OUT.vPos, ProjectionMat);
	OUT.vTex = IN.vTex;
	 
	// Done--return the output.
    return OUT;
}
