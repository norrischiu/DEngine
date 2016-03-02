#define cMult 0.0001002707309736288
#define aSubtract 0.2727272727272727

// globals
cbuffer CONSTANT_BUFFER_PER_FRAME
{
	float4x4 gViewProj;

	float4 gEyePosW;
	float4 gEmitPosW;
	float4 gEmitDirW;

	float gGameTime;
	float gTimeStep;
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


[maxvertexcount(20)]
void GS(point VS_INPUT gin[1],
	inout PointStream<VS_INPUT> ptStream)
{
	gin[0].Age += gTimeStep;

		if (gin[0].Type == PT_EMITTER)
		{
			// time to emit a new particle?
			if (gin[0].Age > 0.005f)
			{
				// temp random vector3
				float3 vRandom = float3(0.1f, 0.1f, 0.1f);
				vRandom.x *= 0.5f;
				vRandom.z *= 0.5f;

				for (int i = 0; i < 1; i++)
				{
					VS_INPUT p;
					//p.InitialPosW = float4(gEmitPosW.xyz, 0.0f);
					p.InitialPosW = rand(float4(gEmitPosW.xyz, 0.0f));
					p.InitialVelW = float4(vRandom, 0.0f);
					p.SizeW = 0.5f;
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
			if (gin[0].Age <= 1.0f)
				ptStream.Append(gin[0]);
		}
	
}