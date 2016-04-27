#pragma once
#include "Graphics/VertexFormat.h"
#include "GameObject/GameObject.h"
#include <vector>

namespace DE
{

class PositioningSystem
{
public:
	struct InitInfo
	{
		Vector3 startXYZ;
		unsigned int GridWidth;
		unsigned int GridDepth;
		float CellSpacing;
	};

	struct Cell
	{
		std::vector<int> positionOwnerId;
	};

	PositioningSystem();
	~PositioningSystem();

	static PositioningSystem* GetInstance();

	void Init(const Vector3& startXYZ, const Vector3& endXYZ, const float CellSpacing = 1.0f);

	const Vector3 GetTransformedPosition(const Vector3& position);
	const Vector3 TransformAndFloor(const Vector3& position);

	std::vector<int> GetPositionOwnerId(const Vector3& position);
	void RemovePositionOwner(const Vector3& position, const int ownerId);
	void UpdatePositionOwner(const Vector3& oldPos, const Vector3& newPos, const int ownerId);

private:
	static PositioningSystem* m_pInstance;
	InitInfo m_initInfo;
	std::vector<PositioningSystem::Cell> m_positioning;
};

};