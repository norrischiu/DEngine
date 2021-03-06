#ifndef DEBUG_H_
#define DEBUG_H_

// Engine include
#include "Math\simdmath.h"

namespace DE
{

class MeshComponent;
class MeshData;

enum Primitives {
	LINE = 2,
	TRIANGULAR_PRISM = 3,
	RECTANGULAR_PRISM = 4,
	PENTAGONAL_PRISM = 5,
	HEXAGONAL_PRISM = 6,
	OCTAGON_PRISM = 8,
	TRIANGULAR_PYRAMID = 3,
	RECTANGULAR_PYRAMID = 4,
	PENTAGONAL_PYRAMID = 5,
	HEXAGONAL_PYRAMID = 6,
	OCTAGON_PYRAMID = 8,
	CYLINDER = 20,
	SPHERE = 20,
	CONE = 20,
	ELLIPSOID = 50,
};

class Debug {

public:
	Debug();
	MeshComponent* draw_pyramid(const Vector3& dimension, const int _slice_, const int stack);
	MeshComponent* draw_prism(const Vector3& dimension, const int stack);
	MeshComponent* draw_ellipsoid(const Vector3& dimension, const int _slice_, const int stack);
	MeshData* draw_line(const Vector3& start_vertex, const Vector3& end_vertex);

	~Debug() {};
};

};
#endif