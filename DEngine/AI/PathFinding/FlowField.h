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

	FlowField(std::vector<std::vector<std::vector<Cell>>> flowField, std::vector<std::vector<std::vector<int>>> dijkstraGrid, const Vector3& m_offset, const Vector3& destination);
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
	std::vector<std::vector<std::vector<int>>> m_dijkstraGrid;
	Vector3 m_offset;
	Vector3 m_destination;
};

};