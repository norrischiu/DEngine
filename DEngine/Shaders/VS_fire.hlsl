#define cMult 0.0001002707309736288
#define aSubtract 0.2727272727272727


cbuffer GS_CONSTANT_BUFFER
{
	// Net constant acceleration used to accerlate the particles.
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };
}

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

float4 randGrieu(float4 t)
{
	float a = t.x + t.z*cMult + aSubtract - floor(t.x);
	a *= a;
	float b = t.y + a;
	b -= floor(b);
	float c = t.z + b;
	c -= floor(c);
	float d = c;
	a += c*cMult + aSubtract - floor(a);
	a *= a;
	b += a;
	b -= floor(b);
	c += b;
	c -= floor(c);
	return float4(a, b, c, d);
}

// function
VS_OUTPUT VS(VS_INPUT vin)
{
	VS_OUTPUT vout;

	float t = vin.Age;

	// constant acceleration equation
	//vout.PosW = vin.InitialPosW;//float4(0.5f*t*t*gAccelW + t*vin.InitialVelW + vin.InitialPosW, 0.0f);

	vout.PosW = float4(0.5f*t*t*gAccelW + t*vin.InitialVelW + vin.InitialPosW, 0.0f);

	// fade color with time  --- smoothstep(min, max, x), x is in the range [min, max].
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
	vout.Color = float4(1.0f, 0.5f, 0.0f, opacity);

	vout.SizeW = vin.SizeW;
	vout.Type = vin.Type;
	vout.Age = vin.Age;

	// temp: useless data
	vout.NoData = 0.0f;

	return vout;
}

