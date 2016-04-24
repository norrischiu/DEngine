// globals
cbuffer CONSTANT_BUFFER_PER_FRAME
{
	float4x4 gViewProj;
	float4x4 gClipToView;
	float4 gEyePosW;
	float4 gEmitPosW;
	float4 gEmitDirW;
	float	gTimeStep;
	float	gDisableTime;
	float	gFlareAge;
	float	gSize;
	unsigned int gMaxParts;
	unsigned int gEffectType;
};

#define PT_EMITTER 0
#define PT_FLARE 1
#define PT_DISABLE 5
#define YELLOW_LIGHT 1
#define BLUE_LIGHT 2
#define BLEEDING 3



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

	// Default case
	float timeToEmit = 0.01f;

	if(gEffectType == YELLOW_LIGHT)
	{
		timeToEmit = 0.01f;
	}
	else if (gEffectType == BLUE_LIGHT)
	{
		timeToEmit = 0.01f;
	}
	else if (gEffectType == BLEEDING)
	{
		timeToEmit = 0.01f;
	}

	if (gin[0].Type == PT_EMITTER)
	{
		// time to emit a new particle?
		if (gin[0].Age > timeToEmit)
		{
			// temp random vector3
			float3 random = rand_1_05(input) * 0.5;
			
			VS_INPUT p;
			//p.InitialPosW = float4(gEmitPosW.xyz + random, 1.0f);
			//p.InitialPosW = float4(gEmitPosW.x + random.x * 0.5, gEmitPosW.y, gEmitPosW.z, 1.0f);
			// Default case
			p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
			p.InitialVelW = float4(gEmitDirW.xyz, 0.0f);

			if (gEffectType == YELLOW_LIGHT)
			{
				p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
				p.InitialVelW = float4(gEmitDirW.xyz, 0.0f);
			}
			else if (gEffectType == BLEEDING)
			{
				p.InitialPosW = float4(gEmitPosW.xyz, 1.0f);
				p.InitialVelW = float4(gEmitDirW.xyz, 0.0f);
				//p.InitialVelW = float4(gEmitDirW.x + random.x * 0.5, gEmitDirW.y, gEmitDirW.z + random.z * 0.5, 0.0f);
			}
					
			p.SizeW = gSize;
			if (gEffectType == 2)
				p.SizeW = gSize + 2.0f;
			p.Age = 0.0f;
			p.Type = PT_FLARE;
			if (gEffectType == BLEEDING)
			{
				if (gDisableTime < 1.0f && gDisableTime > 0.1f)
				{
					p.Type = PT_FLARE;
				}
				else
				{
					p.Type = PT_DISABLE;
				}
			}
			p.NoData = 0;

			ptStream.Append(p);
			
			// reset the time to emit
			gin[0].Age = 0.0f;
		}

		// always keep emitters
		ptStream.Append(gin[0]);
	}
	else
	{
		// Specify conditions to keep particle; this may vary from system to system.

		if (gEffectType == YELLOW_LIGHT)
		{
			if (gin[0].Age <= 0.3f)
				ptStream.Append(gin[0]);
		}
		else if (gEffectType == BLEEDING)
		{
			if (gin[0].Age <= 0.3f)
				ptStream.Append(gin[0]);
		}
		// Default case
		else
		{
			if (gin[0].Age <= 0.3f)
				ptStream.Append(gin[0]);
		}
			
	}
	
}