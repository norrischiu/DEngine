#pragma once
#include <vector>
#include "Graphics/VertexFormat.h"
#include "Graphics/Terrain/Terrain.h"

namespace DE
{

class FlowField
{
public:
	struct Cell {
		Vector3 direction;
		bool isMovable;
		int ownerGameObjId;

		Cell() { }
		Cell(const Vector3& direction, const bool isMovable, const int ownerGameObjId = -1)
			: direction(direction), isMovable(isMovable), ownerGameObjId(ownerGameObjId)
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




	void setPositionDirection(const Vector3& position, const Vector3& direction);

	void setPositionMovable(const Vector3& position, const bool movable);
	bool isPositionMovable(const Vector3& position);

	void setPositionOwnerId(const Vector3& position, const int gameObjId);
	int getPositionOwnerId(const Vector3& position);
	const Vector3 getDirection(const Vector3& currDir, const Vector3& position);

	FlowField::InitInfo getInitInfo();
	std::vector<FlowField::Cell>* getFlowField();
	std::vector<Vector3>* getObstacles();
	const Vector3 getOffset();
	const Vector3 getDestination();

	const Vector3 getTransformedPosition(const Vector3& position);
	const Vector3 transformAndFloor(const Vector3& position);
	bool isValid(const Vector3& position);
	int distance(const Vector3& position, const Vector3& destination);

	void Draw(Terrain* terrain);

private:
	FlowField::InitInfo m_initInfo;
	std::vector<FlowField::Cell> m_flowField;
	std::vector<Vector3> m_obstacles;
	Vector3 m_destination;
	Vector3 m_offset;
};

};