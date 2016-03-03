

// Globals

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

cbuffer GS_CONSTANT_BUFFER
{
	// Net constant acceleration used to accerlate the particles.
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };
}

// Random function
/**
float3 RandUnitVec3(float offset)
{
	// random texture, use to generate random number
	Texture1D gRandomTex;

	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);

	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

	// project onto unit sphere
	return normalize(v);
}*/


// Stream out
#define PT_EMITTER	 0
#define PT_FLARE	 1


struct VS_OUTPUT
{
	float4	PosW  : POSITION;
	float4	Color : COLOR;
	float	SizeW : SIZE;
	float	Age : AGE;
	float 	Type : TYPE;
	float	NoData : DATA;
	// try add two padding to this too, if not okay tell me
};

struct GS_OUTPUT
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
};

// The draw GS just expands points into camera facing quads.
[maxvertexcount(4)]
void GS(point VS_OUTPUT gin[1],
	inout TriangleStream<GS_OUTPUT> triStream)
{
	GS_OUTPUT gout;
	// do not draw emitter particles.

	if (gin[0].Type != PT_EMITTER)
	{
		//
		// Compute world matrix so that billboard faces the camera.
		//

		float3 look = float3(normalize(gEyePosW.xyz - gin[0].PosW.xyz));
		float3 right = normalize(cross(float3(0, 1, 0), look));
		float3 up = cross(look, right);

		//
		// Compute triangle strip vertices (quad) in world space.
		//
		float halfWidth = 0.1f*gin[0].SizeW.x;
		float halfHeight = halfWidth;

		float4 v[4];

			v[0] = float4(gin[0].PosW.xyz + halfWidth*right - halfHeight*up, 1.0f);
			v[1] = float4(gin[0].PosW.xyz + halfWidth*right + halfHeight*up, 1.0f);
			v[2] = float4(gin[0].PosW.xyz - halfWidth*right - halfHeight*up, 1.0f);
			v[3] = float4(gin[0].PosW.xyz - halfWidth*right + halfHeight*up, 1.0f);


		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			gout.PosH = mul(v[i], gViewProj);
			gout.Color = gin[0].Color;
			triStream.Append(gout);
		}
	}
}
