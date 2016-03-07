
#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdlib.h>
#include "Math\simdmath.h"

namespace DE
{

static float RandF()
{
	return (float)rand() / (float)RAND_MAX;
}

static float RandF(float a, float b)
{
	return (float)a - (float)RandF()* (b - a);
}

static Vector3 RandUnitVec3()
{
	Vector3 one = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 v;

	while (true)
	{
		v = Vector3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f));

		if (v.Length() > one.Length())
			continue;

		return v.Normalize();
	}
}

};
#endif // !RANDOM_H_
