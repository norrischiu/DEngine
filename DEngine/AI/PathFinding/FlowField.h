#pragma once
#include <vector>
#include "../../Graphics/VertexFormat.h"

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

	FlowField(std::vector<std::vector<std::vector<Cell>>> flowField, const Vector3& destination);
	~FlowField();

	int getFlowFieldWidth();
	int getFlowFieldHeight();
	int getFlowFieldDepth();

	const Vector3 getDirection(const Vector3& position);
	const Vector3 getDestination();
	bool isPositionMovable(const Vector3& position);

	void print();

private:
	std::vector<std::vector<std::vector<Cell>>> m_flowField;
	Vector3 m_destination;
};

