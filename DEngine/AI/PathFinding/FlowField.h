#pragma once
#include <vector>
#include "Graphics/VertexFormat.h"
#include "GameObject/GameObject.h"
#include "Graphics/Terrain/Terrain.h"

namespace DE
{

class FlowField
{
public:
	struct Cell {
		Vector3 direction;
		bool isMovable;
		GameObject* ownerGameObj;

		Cell() { }
		Cell(const Vector3& direction, const bool isMovable)
			: direction(direction), isMovable(isMovable), ownerGameObj(nullptr)
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

	void setPositionMovable(const Vector3& position, const bool movable);
	bool isPositionMovable(const Vector3& position);

	void setPositionOwner(const Vector3& position, GameObject* gameObj);
	GameObject* getPositionOwner(const Vector3& position);
	const Vector3 getDirection(const Vector3& position);

	std::vector<Vector3>& getObstacles();
	const Vector3 getOffset();
	const Vector3 getDestination();

	const Vector3 transfromAndFloor(const Vector3& position);
	bool isValid(const Vector3& position);
	int distance(const Vector3& position, const Vector3& destination);

	void Draw(Terrain* terrain);

private:
	InitInfo m_initInfo;
	std::vector<Cell> m_flowField;
	std::vector<Vector3> m_obstacles;
	Vector3 m_destination;
	Vector3 m_offset;
	Vector3 m_currDir;
};

};