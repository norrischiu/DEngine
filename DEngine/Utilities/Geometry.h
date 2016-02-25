// Geometry.h: Utility functions related to geometry creation
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Math\simdmath.h"
#include "Physics\cdAabb.h"

namespace Geometry
{
	static float* CreateSpotLightConeVertex() {};

	static unsigned int* CreateSpotLightConeIndex() {};

	static float* CreateUnitAABBVertex() {};

	static unsigned int* CreateUnitAABBIndex() {};

	// Return the simplest bounding box of a list of vertices, use offset to skip unnecessary element
	static AABB CreateAABB(void* inVertices, int size, int offset = sizeof(float))
	{
		float xMax, yMax, zMax, xMin, yMin, zMin;
		xMax = yMax = zMax = xMin = yMin = zMin = 0;
		for (int i = 0; i < size; inVertices = (char*)inVertices + offset, i += sizeof(float) * 4 + offset)
		{
			/*
			xMax = (inVertices[i].GetX() > xMax) ? inVertices[i].GetX() : xMax;
			yMax = (inVertices[i].GetY() > yMax) ? inVertices[i].GetY() : yMax;
			zMax = (inVertices[i].GetZ() > zMax) ? inVertices[i].GetZ() : zMax;
			xMin = (inVertices[i].GetX() < xMin) ? inVertices[i].GetX() : xMin;
			yMin = (inVertices[i].GetY() < yMin) ? inVertices[i].GetY() : yMin;
			zMin = (inVertices[i].GetZ() < zMin) ? inVertices[i].GetZ() : zMin;
			*/
			xMax = *(float*) &inVertices > xMax ? *(float*) &inVertices : xMax;
			xMin = *(float*) &inVertices < xMin ? *(float*) &inVertices : xMin;
			inVertices = (char*) inVertices + sizeof(float);
			yMax = *(float*) &inVertices > yMax ? *(float*) &inVertices : yMax;
			yMin = *(float*) &inVertices < yMin ? *(float*) &inVertices : yMin;
			inVertices = (char*)inVertices + sizeof(float);
			zMax = *(float*) &inVertices > zMax ? *(float*) &inVertices : zMax;
			zMin = *(float*) &inVertices < zMin ? *(float*) &inVertices : zMin;
			inVertices = (char*)inVertices + sizeof(float);
		}
		return AABB(Vector3(xMin, yMin, zMin), Vector3(xMax, yMax, zMax));
	};
}

#endif // GEOMETRY_H_
