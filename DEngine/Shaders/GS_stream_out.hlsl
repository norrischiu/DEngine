// globals
cbuffer CONSTANT_BUFFER_PER_FRAME
{
	float4x4 gViewProj;
	float4x4 gClipToView;
	float4 gEyePosW;
	float4 gEmitPosW;
	float4 gEmitDirW;
	float	gTimeStep;
	float	gFlareAge;
	unsigned int gMaxParts;
	
};

#define PT_EMITTER 0
#define PT_FLARE 1

struct VS_INPUT
{
	float4	InitialPosW : POSITION;
	float4	InitialVelW : VELOCITY;
	float	SizeW : SIZE;
	float	Age : AGE;
	float	Type : TYPE;
	float	NoData : NODATA;
};

float rand_1_05(in float2 uv)
{
	float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233)*2.0)) * 43758.5453));
	return abs(noise.x + noise.y) * 0.5;
}

[maxvertexcount(2)]
void GS(point VS_INPUT gin[1],
	inout PointStream<VS_INPUT> ptStream)
{
	gin[0].Age += gTimeStep;

	float2 input = float2(gTimeStep, gFlareAge);

		if (gin[0].Type == PT_EMITTER)
		{
			// time to emit a new particle?
			if (gin[0].Age > 0.5f)
			{
				// temp random vector3
				float3 random = rand_1_05(input);

				for (int i = 0; i < 1; i++)
				{
					VS_INPUT p;
					//p.InitialPosW = float4(gEmitPosW.xyz, 0.0f);
					p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
					p.InitialVelW = float4(gEmitDirW.xyz*random, 0.0f);
					p.SizeW = 0.4f;
					p.Age = 0.0f;
					p.Type = PT_FLARE;
					p.NoData = 0;

					ptStream.Append(p);
				}
				// reset the time to emit
				gin[0].Age = 0.0f;
			}

			// always keep emitters
			ptStream.Append(gin[0]);
		}
		else
		{
			// Specify conditions to keep particle; this may vary from system to system.
			if (gin[0].Age <= 3.0f)
				ptStream.Append(gin[0]);
		}
	
}