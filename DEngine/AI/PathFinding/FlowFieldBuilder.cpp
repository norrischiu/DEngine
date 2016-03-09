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

void FlowFieldBuilder::setFlowFieldObstacles(std::vector<Vector3> obstacles, const Vector3& offset)
{
	srand(time(NULL));

	for (int i = 0; i < 30; i++)
	{
		// if obstacles are generated in the destination, may cause problem
		int ran1 = rand() % getFlowFieldWidth();
		int ran2 = rand() % getFlowFieldDepth();

		m_flowField[ran1][0][ran2].isMovable = false;
	}

	Matrix4 transform;
	transform.CreateTranslation(offset);

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].Transform(transform);
		const int x = floor(obstacles[i].GetX());
		const int y = floor(obstacles[i].GetY());
		const int z = floor(obstacles[i].GetZ());

		if (
			(x >= 0 && (x <= getFlowFieldWidth() - 1)) ||
			(y >= 0 && (y <= getFlowFieldHeight() - 1)) ||
			(z >= 0 && (z <= getFlowFieldDepth() - 1))
		) {
			m_flowField[x][y][z].isMovable = false;
		}
	}
}

std::vector<std::vector<std::vector<int>>> FlowFieldBuilder::calculateDijkstraGrid(const Vector3& destination, const Vector3& offset)
{
	std::vector<std::vector<std::vector<int>>> dijkstraGrid;

	Matrix4 transform;
	transform.CreateTranslation(Vector3(-offset.GetX(), -offset.GetY(), -offset.GetZ()));
	Vector3 dest = destination;
	dest.Transform(transform);

	const int destX = floor(dest.GetX());
	const int destY = floor(dest.GetY());
	const int destZ = floor(dest.GetZ());

	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		dijkstraGrid.push_back(std::vector<std::vector<int>>());

		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			dijkstraGrid[i].push_back(std::vector<int>());

			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				if (m_flowField[i][j][k].isMovable) {
					dijkstraGrid[i][j].push_back(abs(i - destX) /*+ abs(j - destY)*/ + abs(k - destZ));
				} else {
					dijkstraGrid[i][j].push_back((std::numeric_limits<int>::max)());
				}
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
					!(i == pos.x && j == pos.y && k == pos.z) &&
					(i >= 0 && i <= getFlowFieldWidth() - 1) &&
					(j >= 0 && j <= getFlowFieldHeight() - 1) &&
					(k >= 0 && k <= getFlowFieldDepth() - 1)
				) {
					neighbours.push_back(FlowFieldBuilder::Position(i, j, k));
				}
			}
		}
	}

	return neighbours;
}

void FlowFieldBuilder::setFlowFieldDirection(std::vector<std::vector<std::vector<int>>> dijkstraGrid, const Vector3& offset, const Vector3& destination)
{
	Matrix4 transform;
	transform.CreateTranslation(Vector3(-offset.GetX(), -offset.GetY(), -offset.GetZ()));
	Vector3 dest = destination;
	dest.Transform(transform);

	const int destX = floor(dest.GetX());
	const int destY = floor(dest.GetY());
	const int destZ = floor(dest.GetZ());

	if (
		(destX >= 0 && (destX <= getFlowFieldWidth() - 1)) ||
		(destY >= 0 && (destY <= getFlowFieldHeight() - 1)) ||
		(destZ >= 0 && (destZ <= getFlowFieldDepth() - 1))
	) {
		m_flowField[destX][destY][destZ].isMovable = true;
		m_flowField[destX][destY][destZ].direction = Vector3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				if (i == destX && j == destY && k == destZ) { continue; }

				if (m_flowField[i][j][k].isMovable)
				{
					FlowFieldBuilder::Position pos(i, j, k);
					FlowFieldBuilder::Position minNeighbour;
					auto neighbours = getNeighbours(pos);
					bool isFoundValidNeighbour = false;
					int minDist = getFlowFieldWidth() + getFlowFieldDepth();

					for (int l = 0; l < neighbours.size(); l++)
					{
						FlowFieldBuilder::Position n = neighbours[l];

						if (m_flowField[n.x][n.y][n.z].isMovable)
						{
							const int dist = dijkstraGrid[n.x][n.y][n.z] - dijkstraGrid[pos.x][pos.y][pos.z];

							if (n.x == destX && n.y == destY && n.z == destZ) {			//destination
								minNeighbour = n;
								isFoundValidNeighbour = true;
								break;
							} else if (dist < minDist) {
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

}

FlowField FlowFieldBuilder::generateFlowField(const Vector3& mapMinXYZ, const Vector3& mapMaxXYZ, std::vector<Vector3> obstacles, const Vector3& destination)
{
	const int gridWidth = ceil(abs(mapMaxXYZ.GetX() - mapMinXYZ.GetX()));
	const int gridHeight = ceil(abs(mapMaxXYZ.GetY() - mapMinXYZ.GetY()));
	const int gridDepth = ceil(abs(mapMaxXYZ.GetZ() - mapMinXYZ.GetZ()));
	const Vector3& offset = mapMinXYZ;

	initializeFlowField(Vector3(gridWidth, gridHeight, gridDepth));
	setFlowFieldObstacles(obstacles, offset);
	auto dijkstraGrid = calculateDijkstraGrid(destination, offset);
	setFlowFieldDirection(dijkstraGrid, offset, destination);

	FlowField flowField(m_flowField, dijkstraGrid, offset, destination);
	m_flowField.clear();

	return flowField;
}

FlowField FlowFieldBuilder::generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination)
{
	const AABB mapBoundingBox = map->GetComponent<MeshComponent>()->GetMeshData()->GetBoundingBox();
	const Vector3 mapMinXYZ = mapBoundingBox.getMin();
	const Vector3 mapMaxXYZ = mapBoundingBox.getMax();
	const int gridWidth = ceil(abs(mapMaxXYZ.GetX() - mapMinXYZ.GetX()));
	const int gridHeight = ceil(abs(mapMaxXYZ.GetY() - mapMinXYZ.GetY()));
	const int gridDepth = ceil(abs(mapMaxXYZ.GetZ() - mapMinXYZ.GetZ()));

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
		const Vector3& obsMinXYZ = boundingBox.getMin();
		const Vector3& obsMaxXYZ = boundingBox.getMax();

		const int minX = abs(floor(obsMinXYZ.GetX()));
		const int maxX = abs(floor(obsMaxXYZ.GetX()));
		const int minY = abs(floor(obsMinXYZ.GetY()));
		const int maxY = abs(ceil(obsMaxXYZ.GetY()));
		const int minZ = abs(ceil(obsMinXYZ.GetZ()));
		const int maxZ = abs(ceil(obsMaxXYZ.GetZ()));

		for (int i = minX; i < maxX; i++)
		{
			//for (int j = minY; j < maxY; j++)
			for (int j = 0; j < 1; j++)
			{
				for (int k = minZ; k < maxZ; k++)
				{
					vec3.push_back(Vector3(i, j, k));
				}
			}
		}
	}

	return generateFlowField(mapMinXYZ, mapMaxXYZ, vec3, destination);
}

};
