
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Math\simdmath.h"

struct Particle
{
	Vector3 InitialPos;
	Vector3 InitialVel;
	float Size;
	float Age;
	float Type;
	float NoData;
};


#endif // !PARTICLE_H_
