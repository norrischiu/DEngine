#include "SolidHeightfieldBuilder.h"

SolidHeightfieldBuilder::SolidHeightfieldBuilder()
{
}

SolidHeightfieldBuilder::~SolidHeightfieldBuilder()
{
}

void SolidHeightfieldBuilder::voxelize()
{

}

bool SolidHeightfieldBuilder::isTraversable()
{
	// The top of the span is at least a minimum distance from the bottom of the span above it. (The tallest agent can stand on the span without colliding with an obstruction above.)
	// The top voxel of the span represents geometry with a slope below a maximum allowed value. (The slope is low enough to be traversable by agents.)
	// If ledge culling is enabled, the top of the span does not represent a ledge. (Agents can legally "step down" from the span to all of its neighbors.)

	return true;
}