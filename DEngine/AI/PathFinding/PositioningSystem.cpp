#include "PositioningSystem.h"
#include "Physics/cdAabb.h"

namespace DE
{

PositioningSystem::PositioningSystem(const Vector3& startXYZ, const Vector3& endXYZ, const float CellSpacing)
{
	m_initInfo.startXYZ = startXYZ;
	m_initInfo.GridWidth = (unsigned int)abs(floor(startXYZ.GetX()) - ceil(endXYZ.GetX()));
	m_initInfo.GridDepth = (unsigned int)abs(floor(startXYZ.GetZ()) - ceil(endXYZ.GetZ()));
	m_initInfo.CellSpacing = CellSpacing;

	m_positioning = std::vector<PositioningSystem::Cell>(m_initInfo.GridWidth * m_initInfo.GridDepth);
}

PositioningSystem::~PositioningSystem()
{
}

const Vector3 PositioningSystem::GetTransformedPosition(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_initInfo.startXYZ);
	Vector3 pos = position;
	pos.Transform(transform);

	return pos;
}

const Vector3 PositioningSystem::TransformAndFloor(const Vector3& position)
{
	Vector3 pos = GetTransformedPosition(position);

	pos.SetX(floor(pos.GetX()));
	pos.SetY(floor(pos.GetY()));
	pos.SetZ(floor(pos.GetZ()));

	return pos;
}

std::vector<int> PositioningSystem::GetPositionOwnerId(const Vector3& position)
{
	Vector3 pos = GetTransformedPosition(position);

	const int index = ((int)pos.GetZ()) * m_initInfo.GridWidth + ((int)pos.GetX());

	return m_positioning[index].positionOwnerId;
}

void PositioningSystem::RemovePositionOwner(const Vector3& position, const int ownerId)
{
	Vector3 pos = TransformAndFloor(position);
	int index = (int)pos.GetZ() * m_initInfo.GridWidth + (int)pos.GetX();

	for (auto itr = m_positioning[index].positionOwnerId.begin(); itr != m_positioning[index].positionOwnerId.end();)
	{
		if (*itr == ownerId)
		{
			itr = m_positioning[index].positionOwnerId.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void PositioningSystem::UpdatePositionOwner(const Vector3& oldPos, const Vector3& newPos, const int ownerId)
{
	RemovePositionOwner(oldPos, ownerId);
	RemovePositionOwner(newPos, ownerId);

	Vector3 pos = TransformAndFloor(newPos);
	const int index = (int)pos.GetZ() * m_initInfo.GridWidth + (int)pos.GetX();

	m_positioning[index].positionOwnerId.push_back(ownerId);
}

};
