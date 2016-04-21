#include "GameObjectSpawner.h"
#include "../Graphics/MeshComponent.h"
#include "../Graphics/Scene/SceneGraph.h"
#include "../AI/PathFinding/AIController.h"
#include "../Graphics/Animation/AnimationController.h"
#include "../Object/MovementController.h"

namespace DE
{

GameObjectSpawner::GameObjectSpawner(GameObject* spawnTarget, const int spawnNum, const Vector3& spawnPos, const float spawnTimeDelay)
	: m_spawnTarget(spawnTarget), m_spawnNum(spawnNum), m_spawnPos(spawnPos), m_spawnTimeDelay(spawnTimeDelay), m_accuSpawnNum(0), m_accuSpawnTime(0.0f)
{

}


GameObjectSpawner::~GameObjectSpawner()
{
}

int GameObjectSpawner::Spawn(GameObject* gameObj)
{
	gameObj = new GameObject;

	for (auto itr : *(m_spawnConfig->spawnTarget)->getAllComponents())
	{
		gameObj->AddComponent(itr);
	}

	return 1;
}

void GameObjectSpawner::Update(float deltaTime)
{
	if (m_accuSpawnNum < m_spawnConfig->spawnNum)
	{
		m_accuSpawnTime += deltaTime;

		if (m_accuSpawnTime > (m_accuSpawnNum + 1) * m_spawnConfig->spawnTimeDelay)
		{
			GameObject* gameObj = nullptr;
			const int numGameObj = Spawn(gameObj);

			Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);

			if (m_spawnConfigType == SpawnConfigType::SPAWN_CONFIG_POSITION)
			{
				SpawnConfig_Position* spawnConfig = (SpawnConfig_Position*) m_spawnConfig;
				pos = spawnConfig->positions[m_accuSpawnNum % spawnConfig->numPosition];
			}
			else if (m_spawnConfigType == SpawnConfigType::SPAWN_CONFIG_OFFSET)
			{
				SpawnConfig_Offset* spawnConfig = (SpawnConfig_Offset*)m_spawnConfig;
				pos = (spawnConfig->drawStartPos + spawnConfig->posOffset * m_accuSpawnNum);
			}
			else if (m_spawnConfigType == SpawnConfigType::SPAWN_CONFIG_AREA)
			{
				SpawnConfig_Area* spawnConfig = (SpawnConfig_Area*)m_spawnConfig;

				Vector3 _posOffset_ = spawnConfig->posOffset;

				if (_posOffset_.GetX() < std::numeric_limits<float>::epsilon())
				{
					_posOffset_.SetX(std::numeric_limits<float>::epsilon());
				}

				if (_posOffset_.GetY() < std::numeric_limits<float>::epsilon())
				{
					_posOffset_.SetY(std::numeric_limits<float>::epsilon());
				}

				if (_posOffset_.GetZ() < std::numeric_limits<float>::epsilon())
				{
					_posOffset_.SetZ(std::numeric_limits<float>::epsilon());
				}

				const int numCellX = (int)floor((drawEndPos.GetX() - drawStartPos.GetX()) / _posOffset_.GetX());
				const int numCellY = (int)floor((drawEndPos.GetY() - drawStartPos.GetY()) / _posOffset_.GetY());
				const int numCellZ = (int)floor((drawEndPos.GetZ() - drawStartPos.GetZ()) / _posOffset_.GetZ());

				pos = Vector3(
					(spawnConfig->drawStartPos.GetX() + spawnConfig->posOffset.GetX() * (m_accuSpawnNum % max(numCellX, 1))),
					(spawnConfig->drawStartPos.GetY() + spawnConfig->posOffset.GetY() * (m_accuSpawnNum % (max(numCellX + numCellY, 1))),
					(spawnConfig->drawStartPos.GetZ() + spawnConfig->posOffset.GetZ() * (m_accuSpawnNum % (numCellX + numCellY + numCellZ))
				);
			}


			m_accuSpawnNum++;
		}
	}
	else
	{

	}

}

}