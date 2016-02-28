#include "ContourSetBuilder.h"

ContourSetBuilder::ContourSetBuilder()
{
}


ContourSetBuilder::~ContourSetBuilder()
{
}

void ContourSetBuilder::searchRegionEdges()
{

}

void ContourSetBuilder::vectorize()
{

}

void ContourSetBuilder::optimize()
{
	//Simplify the edges between adjacent polygons. (The portals between regions.)
	//Ensure that border edges conform to the borders of traversable surfaces. (Border edges are the contour edges that connect to empty or obstructed space.)
	//Optimize the length of the border edges. (Borders that are too long can form non-optimal triangles later in the process.)
}