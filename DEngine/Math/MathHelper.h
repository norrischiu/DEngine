#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

namespace DE
{

struct MathHelper
{
	static float Lerpf(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}
};

}
#endif // !MATH_HELPER_H_