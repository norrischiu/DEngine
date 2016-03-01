

// input/output
struct VS_INPUT
{
	float4	InitialPosW : POSITION;
	float4	InitialVelW : VELOCITY;
	float	SizeW		: SIZE;
	float	Age			: AGE;
	float 	Type        : TYPE;
	float	NoData		: NODATA;
};


// function
VS_INPUT VS(VS_INPUT IN)
{
	return IN;
}