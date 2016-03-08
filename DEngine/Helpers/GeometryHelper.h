#ifndef GEOMETRY_HELPER_H_
#define GEOMETRY_HELPER_H_

#include <d3d11.h>
#include "Graphics\VertexFormat.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\MeshComponent.h"

namespace DE
{

enum Primitives 
{
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

class GeometryHelper
{

public:

	GeometryHelper();
	~GeometryHelper();

	Vector3* Pyramid(const Vector3& dimension, const int _slice_, const int stack);
	Vector3* Prism(const Vector3& dimension, const int stack);
	Vector3* Ellipsoid(const Vector3& dimension, const int _slice_, const int stack);
	Vector3* Line(const Vector3& start_vertex, const Vector3& end_vertex);

private:

};

} // namespace DE

#endif // !GEOMETRY_HELPER_H_