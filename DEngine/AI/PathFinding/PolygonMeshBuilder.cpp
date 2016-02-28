#include "PolygonMeshBuilder.h"

PolygonMeshBuilder::PolygonMeshBuilder()
{
}


PolygonMeshBuilder::~PolygonMeshBuilder()
{
}

void PolygonMeshBuilder::merge()
{
	// Triangulation suitable for simple polygons, then merging triangles into the largest possible convex polygons.
	// A configuration parameter (maxVertsPerPoly) that can restrict output to triangles.
}
