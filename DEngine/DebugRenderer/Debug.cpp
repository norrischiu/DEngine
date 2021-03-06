#include "debug.h"
#include "Debug.h"
#include "Graphics\Scene\SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Memory\Handle.h"

namespace DE
{

Debug::Debug()
{
}

MeshData* Debug::draw_line(const Vector3& start_vertex, const Vector3& end_vertex) {

	unsigned int pIndices[2] = {
	0, 1
	};

	Vector3 vertices[2] = {
	start_vertex, end_vertex
	};

	Vector3 dimension = Vector3(
	fabs(start_vertex.GetX() - end_vertex.GetX()),
	fabs(start_vertex.GetY() - end_vertex.GetY()),
	fabs(start_vertex.GetZ() - end_vertex.GetZ())
	);

	MeshData* meshdata = new MeshData(vertices, 2, pIndices, 2);
	return meshdata;
}
/*
MeshComponent* Debug::draw_ellipsoid(const Vector3& dimension, const int slice, const int _stack_) {
>>>>>>> Stashed changes
	float cur_w_radius, cur_d_radius, delta_h, theta = 0.0f;
	const float width = dimension.GetX();
	const float height = dimension.GetY();
	const float depth = dimension.GetZ();
	const float w_radius = width / 2.0f;
	const float d_radius = depth / 2.0f;
	const int stack = _stack_ % 2 == 0 ? _stack_ + 1 : _stack_;
	const int iNumVerts = slice * stack;
	const int iNumIndices = 6 * slice * (stack - 1);
	Vector3* pVertices = new Vector3[iNumVerts];
	unsigned int* pIndices = new unsigned int[iNumIndices];
	const Vector3 center = Vector3(0.0f, 0.0f, 0.0f);

	for (int i = 0, index = 0;
	i < stack;
	i++
	) {
	delta_h = height * i / (stack - 1);
	cur_w_radius = w_radius * cosf(asinf(2 * (height / 2.0f - delta_h) / height));
	cur_d_radius = d_radius * cosf(asinf(2 * (height / 2.0f - delta_h) / height));

	for (int j = 0; j < slice; theta = (2 * PI * ++j) / slice) {
	pVertices[index++] = Vector3(
	cur_w_radius * sinf(theta) + center.GetX(),
	(center.GetY() - height / 2.0f) + delta_h,
	cur_d_radius * cosf(theta) + center.GetZ()
	);
	}
	}

	for (int i = 0, index = 0; index < iNumIndices; i++) {
	for (int j = 0, k = 0; j < 6 * slice; j += 6, k++) {
	if (j != 6 * (slice - 1)) {
	pIndices[index++] = i * slice + k;
	pIndices[index++] = (i + 1) * slice + k;
	pIndices[index++] = i * slice + k + 1;
	pIndices[index++] = (i + 1) * slice + k + 1;
	pIndices[index++] = i * slice + k + 1;
	pIndices[index++] = (i + 1) * slice + k;
	}
	else {
	pIndices[index++] = i * slice;
	pIndices[index++] = (i + 1) * slice - 1;
	pIndices[index++] = (i + 1) * slice;
	pIndices[index++] = (i + 2) * slice - 1;
	pIndices[index++] = (i + 1) * slice;
	pIndices[index++] = (i + 1) * slice - 1;
	}
	}
	}

	MeshData* meshdata = new MeshData(pVertices, iNumVerts, pIndices, iNumIndices);
	//MeshComponent* Meshcomponent = new MeshComponent(meshdata);
	//	SceneGraph::GetInstance()->ADD_DEBUG_DRAWING(Meshcomponent);

	return Meshcomponent;
	}

	MeshComponent* Debug::draw_pyramid(const Vector3& dimension, const int slice, const int _stack_) {

	float cur_w_radius, cur_d_radius, delta_h, theta = 0.0f;
	const float width = dimension.GetX();
	const float height = dimension.GetY();
	const float depth = dimension.GetZ();
	const float w_radius = width / 2.0f;
	const float d_radius = depth / 2.0f;
	const int stack = _stack_ % 2 == 0 ? _stack_ + 1 : _stack_;
	const int iNumVerts = slice * stack;
	const int iNumIndices = 6 * slice * (stack - 1) + (slice - 2) * 3;
	Vector3* pVertices = new Vector3[iNumVerts];
	unsigned int* pIndices = new unsigned int[iNumIndices];
	const Vector3 center = Vector3(0.0f, 0.0f, 0.0f);

	for (int i = 0, index = 0;
	i < stack;
	i++
	) {
	cur_w_radius = (w_radius * (stack - 1 - i)) / (stack - 1);
	cur_d_radius = (d_radius * (stack - 1 - i)) / (stack - 1);
	delta_h = height * i / (stack - 1);

	for (int j = 0; j < slice; theta = (2 * PI * ++j) / slice) {
	pVertices[index++] = Vector3(
	cur_w_radius * sinf(theta) + center.GetX(),
	(center.GetY() - height / 2.0f) + delta_h,
	cur_d_radius * cosf(theta) + center.GetZ()
	);
	}
	}

	for (int i = 0, index = 0; index < 6 * slice * (stack - 1); i++) {
	for (int j = 0, k = 0; j < 6 * slice; j += 6, k++) {
	if (j != 6 * (slice - 1)) {
	pIndices[index++] = i * slice + k;
	pIndices[index++] = (i + 1) * slice + k;
	pIndices[index++] = i * slice + k + 1;
	pIndices[index++] = (i + 1) * slice + k + 1;
	pIndices[index++] = i * slice + k + 1;
	pIndices[index++] = (i + 1) * slice + k;
	}
	else {
	pIndices[index++] = i * slice;
	pIndices[index++] = (i + 1) * slice - 1;
	pIndices[index++] = (i + 1) * slice;
	pIndices[index++] = (i + 2) * slice - 1;
	pIndices[index++] = (i + 1) * slice;
	pIndices[index++] = (i + 1) * slice - 1;
	}
	}
	}

	for (int i = 6 * slice * (stack - 1), j = 0;
	i < iNumIndices;
	j++
	) {
	pIndices[i++] = 0;
	pIndices[i++] = j + 1;
	pIndices[i++] = j + 2;
	}

	MeshData* meshdata = new MeshData(pVertices, iNumVerts, pIndices, iNumIndices);
	//MeshComponent* Meshcomponent = new MeshComponent(meshdata);
	//	SceneGraph::GetInstance()->ADD_DEBUG_DRAWING(Meshcomponent);

	return Meshcomponent;
	}

	MeshComponent* Debug::draw_prism(const Vector3& dimension, const int slice) {
	const float width = dimension.GetX();
	const float height = dimension.GetY();
	const float depth = dimension.GetZ();
	const int iNumVerts = slice * 2;
	const int iNumIndices = 6 * slice + (slice - 2) * 3 * 2;
	Vector3* pVertices = new Vector3[iNumVerts];
	unsigned int* pIndices = new unsigned int[iNumIndices];
	const Vector3 center = Vector3(0.0f, 0.0f, 0.0f);

	float theta = 0.0f;
	for (int i = 0, indexUpper = 0, indexLower = iNumVerts / 2;
	i < slice;
	theta = 2 * PI * ++i / slice
	) {
	pVertices[indexUpper++] = Vector3(
	width / 2.0f * sinf(theta) + center.GetX(),
	center.GetY() + height / 2.0f,
	depth / 2.0f * cosf(theta) + center.GetZ()
	);
	pVertices[indexLower++] = Vector3(
	width / 2.0f * sinf(theta) + center.GetX(),
	center.GetY() - height / 2.0f,
	depth / 2.0f * cosf(theta) + center.GetZ()
	);
	}

	for (int i = 0, j = 0; i < 6 * slice; j++) {
	if (i != 6 * (slice - 1)) {
	pIndices[i++] = j + 0;
	pIndices[i++] = slice + j + 0;
	pIndices[i++] = slice + j + 1;
	pIndices[i++] = slice + j + 1;
	pIndices[i++] = j + 1;
	pIndices[i++] = j + 0;
	}
	else {
	pIndices[i++] = 0;
	pIndices[i++] = slice - 1;
	pIndices[i++] = slice;
	pIndices[i++] = slice * 2 - 1;
	pIndices[i++] = slice;
	pIndices[i++] = slice - 1;
	}
	}

	for (int i = 6 * slice, j = 0;
	i < iNumIndices;
	j++
	) {
	pIndices[i++] = 0;
	pIndices[i++] = j + 1;
	pIndices[i++] = j + 2;
	pIndices[i++] = slice;
	pIndices[i++] = slice + j + 1;
	pIndices[i++] = slice + j + 2;
	}

	MeshData* meshdata = new MeshData(pVertices, iNumVerts, pIndices, iNumIndices);
	//MeshComponent* Meshcomponent = new MeshComponent(meshdata);
	//	SceneGraph::GetInstance()->ADD_DEBUG_DRAWING(Meshcomponent);

	return Meshcomponent;
	}
	*/
};