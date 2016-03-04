#pragma once
#include <vector>
#include "../../Graphics/VertexFormat.h"

namespace DE
{

class FlowField
{
public:
	struct Cell {
		Vector3 direction;
		bool isMovable;

		Cell() { }
		Cell(const Vector3& direction, const bool isMovable)
			: direction(direction), isMovable(isMovable)
		{ }
	};

	int getFlowFieldWidth();
	int getFlowFieldHeight();
	int getFlowFieldDepth();

	const Vector3 getDirection(const Vector3& position);
	bool isPositionMovable(const Vector3& position);

	FlowField(std::vector<std::vector<std::vector<Cell>>> flowField);
	~FlowField();

private:
	std::vector<std::vector<std::vector<Cell>>> m_flowField;
};

};