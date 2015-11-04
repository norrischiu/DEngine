// Let's make this BLUEEEE

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
};

// For now, our vertex shader only needs to ouput
// the correct transformed position.
struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
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
	 
	// Done--return the output.
    return OUT;
}
