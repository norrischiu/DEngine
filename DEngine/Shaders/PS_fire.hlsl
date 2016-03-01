Texture2D shaderTexture;
#define cMult 0.0001002707309736288
#define aSubtract 0.2727272727272727

struct GS_OUTPUT
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
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


// Draw
float4 PS(GS_OUTPUT pin) : SV_TARGET
{
	//float result = shaderTexture.load(int2(3, 4));

	return pin.Color;
}
