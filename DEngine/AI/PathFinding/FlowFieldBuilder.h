#pragma once
#include <vector>
#include "FlowField.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/VertexFormat.h"
#include "../../Physics/cdAABB.h"

namespace DE
{

class FlowFieldBuilder
{
public:
	struct Position {
		int x;
		int y;
		int z;

		Position(const int x, const int y, const int z) : x(x), y(y), z(z) { }
		Position() { }
	};

	FlowFieldBuilder();
	~FlowFieldBuilder();

	static FlowFieldBuilder* getInstance();

	FlowField* generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination, const float cellSpacing = 1.0f);
	FlowField* generateFlowField(const Vector3& minXYZ, const Vector3& maxXYZ, std::vector<GameObject*> obstacles, const Vector3& destination, const float cellSpacing = 1.0f);
	FlowField* generateFlowField(const Vector3& minXYZ, const Vector3& maxXYZ, std::vector<Vector3> obstacles, const Vector3& destination, const float cellSpacing = 1.0f);

private:
	void initializeFlowField(const FlowField::InitInfo initInfo);
	void setFlowFieldObstacles(const FlowField::InitInfo initInfo, std::vector<Vector3> obstacles, const Vector3& offset);
	std::vector<int> calculateDijkstraGrid(const FlowField::InitInfo initInfo, const Vector3& destination, const Vector3& offset);
	std::vector<FlowFieldBuilder::Position> getNeighbours(const FlowField::InitInfo initInfo, const FlowFieldBuilder::Position pos);
	void setFlowFieldDirection(const FlowField::InitInfo initInfo, std::vector<int> dijkstraGrid, const Vector3& offset, const Vector3& destination);

	static FlowFieldBuilder* m_instance;
	std::vector<FlowField::Cell> m_flowField;
};

};
