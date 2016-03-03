#include "FlowFieldBuilder.h"

FlowFieldBuilder::FlowFieldBuilder()
{
}

FlowFieldBuilder::~FlowFieldBuilder()
{
}

int FlowFieldBuilder::getGridWidth()
{
	return m_width;
}

int FlowFieldBuilder::getGridHeight()
{
	return m_height;
}

int FlowFieldBuilder::getGridDepth()
{
	return m_depth;
}

void FlowFieldBuilder::generateFlowField(MeshComponent* map, std::vector<MeshComponent*> obstacles)
{
	const AABB& mapBoundingBox = map->m_pMeshData->GetBoundingBox();
	const Vector3 min = mapBoundingBox.getMin();
	const Vector3 max = mapBoundingBox.getMax();

	const int gridWidth = mapBoundingBox.getMax().GetX() - mapBoundingBox.getMin().GetX();
	const int gridHeight ;
	const int gridDepth;
}
