#define cMult 0.0001002707309736288
#define aSubtract 0.2727272727272727
#define TORCH_FLAME 1
#define SMOKE 2
#define ROCKET_TRAIL 3
#define FIRE 4

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4	gViewProj;
	float4x4	gClipToView;
	float4		gEyePosW;

	float4		gEmitPosW;
	float4		gEmitDirW;

	float		gTimeStep;
	float		gFlareAge;
	float	gSize;
	unsigned int gMaxParts;
	unsigned int gEffectType;
};


// input (Particle)
struct VS_INPUT
{
	float4	InitialPosW : POSITION;
	float4	InitialVelW : VELOCITY;
	float	SizeW : SIZE;
	float	Age : AGE;
	float 	Type : TYPE;
	float	NoData : NODATA;
};

// output
struct VS_OUTPUT
{
	float4	PosW  : POSITION;
	float4	Color : COLOR;
	float	SizeW : SIZE;
	float	Age : AGE;
	float 	Type : TYPE;
	float	NoData : DATA;
};

// function
VS_OUTPUT VS(VS_INPUT vin)
{
	VS_OUTPUT vout;

	
	float3 gAccelW = { 0.0f, 0.0f, 0.0f };
	float t = vin.Age;
	if (gEffectType == TORCH_FLAME)
	{
		vout.SizeW = vin.SizeW - t * vin.SizeW / 1.8;

	}
	else if (gEffectType == SMOKE)
	{
		vout.SizeW = t * vin.SizeW / 1.8 /2;
	}
	else if (gEffectType == FIRE)
	{
		vout.SizeW = t * vin.SizeW / 1.8;
	}
	vout.PosW = float4(0.5f*t*t*gAccelW + t*vin.InitialVelW + vin.InitialPosW, 0.0f);
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
	vout.Color = float4(1.0f, 0.5f, 0.0f, opacity);
	vout.Type = vin.Type;
	vout.Age = vin.Age;

	// temp: useless data
	vout.NoData = 0.0f;
	
	return vout;
}

