#pragma once
#include <vector>
#include "FlowField.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/VertexFormat.h"

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

	FlowField generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination);

private:
	int getFlowFieldWidth();
	int getFlowFieldHeight();
	int getFlowFieldDepth();

	void initializeFlowField(GameObject* map);
	void setFlowFieldObstacles(std::vector<GameObject*> obstacles);
	std::vector<std::vector<std::vector<int>>> calculateDijkstraGrid(const Vector3& destination);
	std::vector<FlowFieldBuilder::Position> getNeighbours(FlowFieldBuilder::Position pos);
	void setFlowFieldDirection(std::vector<std::vector<std::vector<int>>> dijkstraGrid);

	static FlowFieldBuilder* m_instance;
	std::vector<std::vector<std::vector<FlowField::Cell>>> m_flowField;
};

};