#include "FlowFieldBuilder.h"
#include "../../Graphics/MeshComponent.h"
#include "../../Physics/cdAABB.h"

FlowFieldBuilder* FlowFieldBuilder::m_instance = nullptr;

FlowFieldBuilder::FlowFieldBuilder()
{
}

FlowFieldBuilder::~FlowFieldBuilder()
{
}

FlowFieldBuilder* FlowFieldBuilder::getInstance()
{
	if (!m_instance) {
		m_instance = new FlowFieldBuilder();
	}

	return m_instance;
}

int FlowFieldBuilder::getFlowFieldWidth()
{
	return m_flowField.size();
}

int FlowFieldBuilder::getFlowFieldHeight()
{
	return m_flowField[0].size();
}

int FlowFieldBuilder::getFlowFieldDepth()
{
	return m_flowField[0][0].size();
}

void FlowFieldBuilder::initializeFlowField(GameObject* map)
{
	//Grid configuration
	const AABB mapBoundingBox = map->GetComponent<MeshComponent>()->GetMeshData()->GetBoundingBox();
	const Vector3 min = mapBoundingBox.getMin();
	const Vector3 max = mapBoundingBox.getMax();
	const int gridWidth = (int)ceil(abs(max.GetX() - min.GetX()));
	const int gridHeight = (int)ceil(abs(max.GetY() - min.GetY()));
	const int gridDepth = (int)ceil(abs(max.GetZ() - min.GetZ()));

	//Initialize the grid
	for (int i = 0; i < gridWidth; i++)
	{
		m_flowField.push_back(std::vector<std::vector<FlowField::Cell>>());

		// for (int j = 0; j < gridHeight; j++)
		for (int j = 0; j < 1; j++)
		{
			m_flowField[i].push_back(std::vector<FlowField::Cell>());

			for (int k = 0; k < gridDepth; k++)
			{
				m_flowField[i][j].push_back(FlowField::Cell(Vector3(0.0f, 0.0f, 0.0f), true));
			}
		}
	}
}

void FlowFieldBuilder::setFlowFieldObstacles(std::vector<GameObject*> obstacles)
{
	//Find obstacles
	for (int i = 0; i < obstacles.size(); i++)
	{
		const AABB boundingBox = obstacles[i]->GetComponent<MeshComponent>()->GetMeshData()->GetBoundingBox();

		const Vector3& max = boundingBox.getMax();
		const Vector3& min = boundingBox.getMin();
		const int minX = (int)min.GetX();
		const int maxX = (int)ceil(max.GetX());
		const int minY = (int)min.GetY();
		const int maxY = (int)ceil(max.GetY());
		const int minZ = (int)min.GetZ();
		const int maxZ = (int)ceil(max.GetZ());

		for (int i = minX; i < maxX; i++)
		{
			//for (int j = minY; j < maxY; j++)
			for (int j = 0; j < 1; j++)
			{
				for (int k = minZ; k < maxZ; k++)
				{
					if (
						(i >= 0 && (i <= getFlowFieldWidth() - 1)) ||
						(j >= 0 && (j <= getFlowFieldHeight() - 1)) ||
						(k >= 0 && (k <= getFlowFieldDepth() - 1))
					) {
						m_flowField[i][j][k].isMovable = false;
					}
				}
			}
		}
	}
}

std::vector<std::vector<std::vector<int>>> FlowFieldBuilder::calculateDijkstraGrid(const Vector3& destination)
{
	std::vector<std::vector<std::vector<int>>> dijkstraGrid;
	const int destX = destination.GetX();
	const int destY = destination.GetY();
	const int destZ = destination.GetZ();

	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		dijkstraGrid.push_back(std::vector<std::vector<int>>());

		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			dijkstraGrid[i].push_back(std::vector<int>());

			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				dijkstraGrid[i][j].push_back(abs(i - destX) /*+ abs(j - destY)*/ + abs(k - destZ));
			}
		}
	}

	return dijkstraGrid;
}

std::vector<FlowFieldBuilder::Position> FlowFieldBuilder::getNeighbours(FlowFieldBuilder::Position pos)
{
	std::vector<FlowFieldBuilder::Position> neighbours;

	for (int i = pos.x - 1; i <= pos.x + 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = pos.z - 1; k <= pos.z + 1; k++)
			{
				if (
					(i >= 0 && i <= getFlowFieldWidth() - 1) &&
					(j >= 0 && j <= getFlowFieldHeight() - 1) &&
					(k >= 0 && k <= getFlowFieldDepth() - 1) &&
					m_flowField[i][j][k].isMovable
				) {
					neighbours.push_back(FlowFieldBuilder::Position(i, j, k));
				}
			}
		}
	}

	return neighbours;
}

void FlowFieldBuilder::setFlowFieldDirection(std::vector<std::vector<std::vector<int>>> dijkstraGrid)
{
	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				FlowFieldBuilder::Position pos(i, j, k);
				FlowFieldBuilder::Position minNeighbour;
				auto neighbours = getNeighbours(pos);
				bool isFoundValidNeighbour = false;
				int minDist = 0;
				
				for (int l = 0; l < neighbours.size(); l++)
				{
					FlowFieldBuilder::Position n = neighbours[l];
					if (m_flowField[n.x][n.y][n.z].isMovable)
					{
						const int dist = dijkstraGrid[n.x][n.y][n.z] - dijkstraGrid[pos.x][pos.y][pos.z];

						if (dist < minDist) {
							minDist = dist;
							minNeighbour = n;
							isFoundValidNeighbour = true;
						}
					}
				}

				if (isFoundValidNeighbour)
				{
					m_flowField[i][j][k].direction = (Vector3(minNeighbour.x, minNeighbour.y, minNeighbour.z) - Vector3(pos.x, pos.y, pos.z)).Normalize();
				}
			}
		}
	}
}

FlowField FlowFieldBuilder::generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination)
{

	initializeFlowField(map);
	setFlowFieldObstacles(obstacles);
	auto dijkstraGrid = calculateDijkstraGrid(destination);
	setFlowFieldDirection(dijkstraGrid);

	FlowField flowField(m_flowField);
	m_flowField.clear();

	return flowField;
}
