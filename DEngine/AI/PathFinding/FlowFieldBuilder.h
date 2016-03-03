#pragma once
#include <vector>
#include "../../Graphics/MeshComponent.h"
#include "../../Graphics/VertexFormat.h"

class FlowFieldBuilder
{
public:
	struct Cell {
		Vector3 direction;
		bool isBlocked;
	};

	FlowFieldBuilder();
	~FlowFieldBuilder();

	int getGridWidth();
	int getGridHeight();
	int getGridDepth();

	void generateFlowField(MeshComponent* map, std::vector<MeshComponent*> obstacles);

private:
	std::vector<std::vector<std::vector<Cell>>> m_flowField;
	int m_width;
	int m_height;
	int m_depth;
};

