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
	float	gSize;
	unsigned int gMaxParts;
	unsigned int gEffectType;
};

#define PT_EMITTER 0
#define PT_FLARE 1

enum 
{
	YELLOW_LIGHT = 1,
	SMOKE = 2,
	ROCKET_TRAIL = 3,
	FIRE = 4,
};

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
			if (gin[0].Age > 0.01f)
			{
				// temp random vector3
				float3 random = rand_1_05(input) * 0.5;

				for (int i = 0; i < 1; i++)
				{
					VS_INPUT p;
					//p.InitialPosW = float4(gEmitPosW.xyz + random, 1.0f);
					//p.InitialPosW = float4(gEmitPosW.x + random.x * 0.5, gEmitPosW.y, gEmitPosW.z, 1.0f);
					
					switch(gEffectType):
						case YELLOW_LIGHT:
							p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
							break;
						default:
							p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
			
					p.InitialVelW = float4(gEmitDirW.xyz, 0.0f);
					p.SizeW = gSize;
					if (gEffectType == 2)
						p.SizeW = gSize + 2.0f;
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
			if (gin[0].Age <= 0.3f)
				ptStream.Append(gin[0]);
		}
	
}