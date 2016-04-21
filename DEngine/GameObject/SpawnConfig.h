#pragma once
#include "GameObject.h"
#include "../Graphics/VertexFormat.h"

namespace DE
{

enum SpawnConfigType
{
	SPAWN_CONFIG_POSITION,
	SPAWN_CONFIG_OFFSET,
	SPAWN_CONFIG_AREA
};

struct SpawnConfig
{
	SpawnConfig(GameObject* spawnTarget, const unsigned int spawnNum, const float spawnTimeDelay) :
		spawnTarget(spawnTarget),
		spawnNum(spawnNum),
		spawnTimeDelay(spawnTimeDelay)
	{}

	public:
		GameObject* spawnTarget;
		unsigned int spawnNum;
		float spawnTimeDelay;
};

struct SpawnConfig_Position : public SpawnConfig
{
	SpawnConfig_Position(GameObject* spawnTarget, const unsigned int spawnNum, const float spawnTimeDelay, Vector3* positions, const unsigned int numPosition) :
		SpawnConfig(spawnTarget, spawnNum, spawnTimeDelay),
		positions(positions) 
	{}

	public:
		Vector3* positions;
		unsigned int numPosition;
};

struct SpawnConfig_Offset : public SpawnConfig
{
	SpawnConfig_Offset(GameObject* spawnTarget, const unsigned int spawnNum, const float spawnTimeDelay, const Vector3& drawStartPos, const Vector3& offset) :
		SpawnConfig(spawnTarget, spawnNum, spawnTimeDelay),
		drawStartPos(drawStartPos),
		posOffset(posOffset)
	{}

	public:
		Vector3 drawStartPos;
		Vector3 posOffset;
};

struct SpawnConfig_Area : public SpawnConfig_Offset
{
	SpawnConfig_Area(GameObject* spawnTarget, const unsigned int spawnNum, const float spawnTimeDelay, const Vector3& drawStartPos, const Vector3& drawEndPos, const Vector3& posOffset) :
		SpawnConfig_Offset(spawnTarget, spawnNum, spawnTimeDelay, drawStartPos, posOffset),
		drawEndPos(drawEndPos)
	{
		unsigned int _spawnNum_ = 0;
		float offsetX = posOffset.GetX();
		float offsetZ = posOffset.GetZ();

		if (offsetX < std::numeric_limits<float>::epsilon())
		{
			offsetX = std::numeric_limits<float>::epsilon();
		}

		if (offsetZ < std::numeric_limits<float>::epsilon())
		{
			offsetZ = std::numeric_limits<float>::epsilon();
		}

		const int numCellX = (int)floor((drawEndPos.GetX() - drawStartPos.GetX()) / offsetX);
		const int numCellZ = (int)floor((drawEndPos.GetZ() - drawStartPos.GetZ()) / offsetZ);

		if (numCellX + numCellX > 0)
		{
			_spawnNum_ = (numCellX == 0 ? 1 : numCellX) * (numCellZ == 0 ? 1 : numCellZ);
		}

		if (SpawnConfig_Offset::spawnNum != _spawnNum_)
		{
			SpawnConfig_Offset::spawnNum = _spawnNum_;
		}
	}

	public:
		Vector3 drawEndPos;
};

};