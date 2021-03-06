#pragma once
#include <vector>
#include "Graphics/VertexFormat.h"

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

	struct InitInfo {
		int FlowFieldWidth;
		int FlowFieldDepth;
		float CellSpacing;

		InitInfo(const int FlowFieldWidth, const int FlowFieldDepth, const float CellSpacing)
			: FlowFieldWidth(FlowFieldWidth), FlowFieldDepth(FlowFieldDepth), CellSpacing(CellSpacing) { }

		InitInfo() {}
	};

	FlowField(const InitInfo initInfo, std::vector<Cell> flowField, std::vector<Vector3> m_obstacles, const Vector3& m_offset, const Vector3& destination);
	~FlowField();

	bool isPositionMovable(const Vector3& position);
	const Vector3 getDirection(const Vector3& position);

	std::vector<Vector3>& getObstacles();
	const Vector3 getOffset();
	const Vector3 getDestination();

	const Vector3 transfromAndFloor(const Vector3& position);
	bool isValid(const Vector3& position);
	int distance(const Vector3& position, const Vector3& destination);

	void Draw();

private:
	InitInfo m_initInfo;
	std::vector<Cell> m_flowField;
	std::vector<Vector3> m_obstacles;
	Vector3 m_destination;
	Vector3 m_offset;
	Vector3 m_currDir;
};

};