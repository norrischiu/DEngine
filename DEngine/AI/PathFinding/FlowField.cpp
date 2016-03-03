#include "FlowField.h"

FlowField::FlowField(std::vector<std::vector<std::vector<Cell>>> flowField)
	: m_flowField(flowField)
{
}

FlowField::~FlowField()
{
}

int FlowField::getFlowFieldWidth()
{
	return m_flowField.size();
}

int FlowField::getFlowFieldHeight()
{
	return m_flowField[0].size();
}

int FlowField::getFlowFieldDepth()
{
	return m_flowField[0][0].size();
}

bool FlowField::isPositionMovable(const Vector3& position)
{
	const int x = position.GetX();
	const int y = position.GetY();
	const int z = position.GetZ();

	if (
		(x >= 0 && x <= getFlowFieldWidth() - 1) &&
		(y >= 0 && y <= getFlowFieldHeight() - 1) &&
		(z >= 0 && z <= getFlowFieldDepth() - 1)
	) {
		return m_flowField[x][y][z].isMovable;
	}

	return false;
}

const Vector3 FlowField::getDirection(const Vector3& position)
{
	const int x = position.GetX();
	const int y = position.GetY();
	const int z = position.GetZ();

	if (
		(x >= 0 && x <= getFlowFieldWidth() - 1) &&
		(y >= 0 && y <= getFlowFieldHeight() - 1) &&
		(z >= 0 && z <= getFlowFieldDepth() - 1) &&
		m_flowField[x][y][z].isMovable
	) {
		return m_flowField[x][y][z].direction;
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}
