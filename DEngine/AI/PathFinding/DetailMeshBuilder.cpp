#include "DetailMeshBuilder.h"

DetailMeshBuilder::DetailMeshBuilder()
{
}


DetailMeshBuilder::~DetailMeshBuilder()
{
}

void DetailMeshBuilder::triangulate()
{
	// The convex polygon mesh is triangulated using Delaunay triangulation so that height detail can be added.Vertices are added internally and to the edges of polygons to ensure the original geometry's surface is adequately followed.
	// At the end of this step, the traversable surface is represented by a mesh of triangles matched to the height contours of the source geometry.
}
