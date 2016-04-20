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

void FlowFieldBuilder::initializeFlowField(const FlowField::InitInfo initInfo)
{
	//Initialize the grid
	for (int z = 0; z <  initInfo.FlowFieldDepth; z++)
	{
		for (int x = 0; x < initInfo.FlowFieldWidth; x++)
		{
			m_flowField.push_back(FlowField::Cell(Vector3(0.0f, 0.0f, 0.0f), true));
		}
	}
}

void FlowFieldBuilder::setFlowFieldObstacles(const FlowField::InitInfo initInfo, std::vector<Vector3> obstacles, const Vector3& offset)
{
	Matrix4 transform;
	transform.CreateTranslation(offset);

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].Transform(transform);
		const int x = floor(obstacles[i].GetX());
		const int z = floor(obstacles[i].GetZ());

		if (
			(x >= 0 && (x <= initInfo.FlowFieldWidth - 1)) &&
			(z >= 0 && (z <= initInfo.FlowFieldDepth - 1))
		) {
			const int index = z * initInfo.FlowFieldWidth + x;
			m_flowField[index].isMovable = false;
		}
	}
}

std::vector<int> FlowFieldBuilder::calculateDijkstraGrid(const FlowField::InitInfo initInfo, const Vector3& destination, const Vector3& offset)
{
	std::vector<int> dijkstraGrid(initInfo.FlowFieldWidth * initInfo.FlowFieldDepth);

	Matrix4 transform;
	transform.CreateTranslation(Vector3(-offset.GetX(), -offset.GetY(), -offset.GetZ()));
	Vector3 dest = destination;
	dest.Transform(transform);

	const int destX = floor(dest.GetX());
	const int destZ = floor(dest.GetZ());

	for (int z = 0; z < initInfo.FlowFieldDepth; z++)
	{
		for (int x = 0; x < initInfo.FlowFieldWidth; x++)
		{
			const int index = z * initInfo.FlowFieldWidth + x;

			if (m_flowField[index].isMovable) {
				dijkstraGrid[index] = abs(x - destX) + abs(z - destZ);
			} else {
				dijkstraGrid[index] = (std::numeric_limits<int>::max)();
			}
		}
	}

	return dijkstraGrid;
}

std::vector<FlowFieldBuilder::Position> FlowFieldBuilder::getNeighbours(const FlowField::InitInfo initInfo, const FlowFieldBuilder::Position pos)
{
	std::vector<FlowFieldBuilder::Position> neighbours;

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		for (int z = pos.z - 1; z <= pos.z + 1; z++)
		{
			if (
				!(x == pos.x && z == pos.z) &&
				(x >= 0 && x <= initInfo.FlowFieldWidth - 1) &&
				(z >= 0 && z <= initInfo.FlowFieldDepth - 1)
			) {
				neighbours.push_back(FlowFieldBuilder::Position(x, 0, z));
			}
		}
	}

	return neighbours;
}

void FlowFieldBuilder::setFlowFieldDirection(const FlowField::InitInfo initInfo, std::vector<int> dijkstraGrid, const Vector3& offset, const Vector3& destination)
{
	Matrix4 transform;
	transform.CreateTranslation(Vector3(-offset.GetX(), -offset.GetY(), -offset.GetZ()));
	Vector3 dest = destination;
	dest.Transform(transform);

	const int destX = floor(dest.GetX());
	const int destZ = floor(dest.GetZ());

	if (
		(destX >= 0 && (destX <= initInfo.FlowFieldWidth - 1)) &&
		(destZ >= 0 && (destZ <= initInfo.FlowFieldDepth - 1))
	) {
		const int index = destZ * initInfo.FlowFieldWidth + destX;
		m_flowField[index].isMovable = true;
		m_flowField[index].direction = Vector3(0.0f, 0.0f, 0.0f);
	}

	for (int z = 0; z < initInfo.FlowFieldDepth; z++)
	{
		for (int x = 0; x < initInfo.FlowFieldWidth; x++)
		{
			if (x == destX && z == destZ) { continue; }

			const int index = z * initInfo.FlowFieldWidth + x;

			if (m_flowField[index].isMovable)
			{
				FlowFieldBuilder::Position pos(x, 0, z);
				FlowFieldBuilder::Position minNeighbour;
				auto neighbours = getNeighbours(initInfo, pos);
				bool isFoundValidNeighbour = false;
				int minDist = initInfo.FlowFieldWidth + initInfo.FlowFieldDepth;

				for (int l = 0; l < neighbours.size(); l++)
				{
					FlowFieldBuilder::Position n = neighbours[l];

					if (m_flowField[n.z * initInfo.FlowFieldWidth + n.x].isMovable)
					{
						const int dist = dijkstraGrid[n.z * initInfo.FlowFieldWidth + n.x] - dijkstraGrid[pos.z * initInfo.FlowFieldWidth + pos.x];

						if (n.x == destX && n.z == destZ) {			//destination
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
					m_flowField[index].direction = (Vector3(minNeighbour.x, minNeighbour.y, minNeighbour.z) - Vector3(pos.x, pos.y, pos.z)).Normalize();
				}
			}
		}
	}

}

FlowField* FlowFieldBuilder::generateFlowField(const Vector3& mapMinXYZ, const Vector3& mapMaxXYZ, std::vector<Vector3> obstacles, const Vector3& destination, const float cellSpacing)
{
	const int gridWidth = ceil(abs(mapMaxXYZ.GetX() - mapMinXYZ.GetX()));
	const int gridDepth = ceil(abs(mapMaxXYZ.GetZ() - mapMinXYZ.GetZ()));
	const Vector3& offset = mapMinXYZ;

	FlowField::InitInfo initInfo(gridWidth, gridDepth, cellSpacing);

	initializeFlowField(initInfo);
	setFlowFieldObstacles(initInfo, obstacles, offset);
	auto dijkstraGrid = calculateDijkstraGrid(initInfo, destination, offset);
	setFlowFieldDirection(initInfo, dijkstraGrid, offset, destination);

	FlowField* flowField = new FlowField(initInfo, m_flowField, obstacles, offset, destination);
	m_flowField.clear();

	return flowField;
}

FlowField* FlowFieldBuilder::generateFlowField(const Vector3& mapMinXYZ, const Vector3& mapMaxXYZ, std::vector<GameObject*> obstacles, const Vector3& destination, const float cellSpacing)
{
	std::vector<Vector3> vec3;

	//Find obstacles
	for (int i = 0; i < obstacles.size(); i++)
	{
		const AABB boundingBox = obstacles[i]->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
		const Vector3& obsMinXYZ = boundingBox.getMin();
		const Vector3& obsMaxXYZ = boundingBox.getMax();

		const int minX = abs(floor(obsMinXYZ.GetX()));
		const int maxX = abs(floor(obsMaxXYZ.GetX()));
		const int minZ = abs(ceil(obsMinXYZ.GetZ()));
		const int maxZ = abs(ceil(obsMaxXYZ.GetZ()));

		for (int x = minX; x < maxX; x++)
		{
			for (int z = minZ; z < maxZ; z++)
			{
				vec3.push_back(Vector3(x, 0.0f, z));
			}
		}
	}

	return generateFlowField(mapMinXYZ, mapMaxXYZ, vec3, destination);
}

FlowField* FlowFieldBuilder::generateFlowField(GameObject* map, std::vector<GameObject*> obstacles, const Vector3& destination, const float cellSpacing)
{
	const AABB mapBoundingBox = map->GetComponent<MeshComponent>()->GetMeshData()->GetBoundingBox();
	const Vector3 mapMinXYZ = mapBoundingBox.getMin();
	const Vector3 mapMaxXYZ = mapBoundingBox.getMax();
	const int gridWidth = ceil(abs(mapMaxXYZ.GetX() - mapMinXYZ.GetX()));
	const int gridDepth = ceil(abs(mapMaxXYZ.GetZ() - mapMinXYZ.GetZ()));

	std::vector<Vector3> vec3;

	//Find obstacles
	for (int i = 0; i < obstacles.size(); i++)
	{
		const AABB boundingBox = obstacles[i]->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
		const Vector3& obsMinXYZ = boundingBox.getMin();
		const Vector3& obsMaxXYZ = boundingBox.getMax();

		const int minX = abs(floor(obsMinXYZ.GetX()));
		const int maxX = abs(floor(obsMaxXYZ.GetX()));
		const int minZ = abs(ceil(obsMinXYZ.GetZ()));
		const int maxZ = abs(ceil(obsMaxXYZ.GetZ()));

		for (int x = minX; x < maxX; x++)
		{
			for (int z = minZ; z < maxZ; z++)
			{
				vec3.push_back(Vector3(x, 0.0f, z));
			}
		}
	}

	return generateFlowField(mapMinXYZ, mapMaxXYZ, vec3, destination);
}

};
