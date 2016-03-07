#include "FlowFieldBuilder.h"
#include "../../Graphics/MeshComponent.h"
#include <time.h>

namespace DE
{

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

void FlowFieldBuilder::initializeFlowField(const Vector3& map_dimension)
{
	//Grid configuration
	const int gridWidth = map_dimension.GetX();
	const int gridHeight = map_dimension.GetY();
	const int gridDepth = map_dimension.GetZ();

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

void FlowFieldBuilder::setFlowFieldObstacles(std::vector<Vector3> obstacles)
{
	srand(time(NULL));

	for (int i = 0; i < 15; i++)
	{
		int ran1 = rand() % 10;
		int ran2 = rand() % 10;

		m_flowField[ran1][0][ran2].isMovable = false;
	}

	Matrix4 transform;
	transform.CreateTranslation(Vector3(getFlowFieldWidth() / 2.0f, 0.0f, getFlowFieldDepth() / 2.0f));

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].Transform(transform);
		const int x = obstacles[i].GetX();
		const int y = obstacles[i].GetY();
		const int z = obstacles[i].GetZ();

		if (
			(x >= 0 && (x <= getFlowFieldWidth() - 1)) ||
			(y >= 0 && (y <= getFlowFieldHeight() - 1)) ||
			(z >= 0 && (z <= getFlowFieldDepth() - 1))
		) {
			m_flowField[x][y][z].isMovable = false;
		}
		
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

	Matrix4 transform;
	transform.CreateTranslation(Vector3(getFlowFieldWidth() / 2.0f, 0.0f, getFlowFieldDepth() / 2.0f));
	Vector3 dest = destination;
	dest.Transform(transform);

	const int destX = abs(dest.GetX());
	const int destY = abs(dest.GetY());
	const int destZ = abs(dest.GetZ());

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

FlowField FlowFieldBuilder::generateFlowField(const Vector3& map_dimension, std::vector<Vector3> obstacles, const Vector3& destination)
{
	initializeFlowField(map_dimension);
	setFlowFieldObstacles(obstacles);
	auto dijkstraGrid = calculateDijkstraGrid(destination);
	setFlowFieldDirection(dijkstraGrid);

	FlowField flowField(m_flowField, destination);
	m_flowField.clear();

	return flowField;
}

<<<<<<< HEAD
FlowField FlowFieldBuilder::generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination)
{
	const AABB mapBoundingBox = map->GetComponent<MeshComponent>()->GetMeshData()->GetBoundingBox();
	const Vector3 min = mapBoundingBox.getMin();
	const Vector3 max = mapBoundingBox.getMax();
	const int gridWidth = ceil(abs(max.GetX() - min.GetX()));
	const int gridHeight = ceil(abs(max.GetY() - min.GetY()));
	const int gridDepth = ceil(abs(max.GetZ() - min.GetZ()));

	std::vector<AABB> boundBoxes;
	std::vector<Vector3> vec3;

	for (int i = 0; i < obstacles.size(); i++)
	{
		boundBoxes.push_back(obstacles[i]->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox());
	}

	//Find obstacles
	for (int i = 0; i < obstacles.size(); i++)
	{
		const AABB boundingBox = obstacles[i]->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
		Vector3 max = boundingBox.getMax();
		Vector3 min = boundingBox.getMin();

		const int minX = abs((int)min.GetX());
		const int maxX = abs((int)max.GetX());
		const int minY = abs((int)min.GetY());
		const int maxY = abs((int)max.GetY());
		const int minZ = abs((int)min.GetZ());
		const int maxZ = abs((int)max.GetZ());

		for (int i = minX; i <= maxX; i++)
		{
			//for (int j = minY; j < maxY; j++)
			for (int j = 0; j < 1; j++)
			{
				for (int k = minZ; k <= maxZ; k++)
				{
					vec3.push_back(Vector3(i, j, k));
				}
			}
		}
	}

	return generateFlowField(Vector3(gridWidth, gridHeight, gridDepth), vec3, destination);
}
=======
};
>>>>>>> 76d96a33a44b8046f7812c39b0205fa5fe9e8149
