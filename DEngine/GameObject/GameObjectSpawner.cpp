#include "GameObjectSpawner.h"
#include "../Graphics/Scene/SceneGraph.h"

namespace DE
{

GameObjectSpawner::GameObjectSpawner(SpawnConfig* spawnConfig, SpawnConfigType spawnConfigType, Terrain* terrain)
	: m_spawnConfig(spawnConfig), m_spawnConfigType(spawnConfigType), m_terrain(terrain)
{

}


GameObjectSpawner::~GameObjectSpawner()
{
}

int GameObjectSpawner::Spawn(GameObject*& gameObj)
{
	gameObj = new GameObject;

	for (auto itr : *(m_spawnConfig->spawnTarget)->getAllComponents())
	{
		gameObj->AddComponent(itr);

		if (itr->GetID() == ComponentID::MESH)
		{
			DE::SceneGraph::GetInstance()->AddComponent((DE::MeshComponent*) itr);
		}
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

			Vector3 pos = m_spawnConfig->spawnTarget->GetPosition();

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

				float offsetX = spawnConfig->posOffset.GetX();
				float offsetZ = spawnConfig->posOffset.GetZ();

				if (offsetX < std::numeric_limits<float>::epsilon())
				{
					offsetX = std::numeric_limits<float>::epsilon();
				}

				if (offsetZ < std::numeric_limits<float>::epsilon())
				{
					offsetZ = std::numeric_limits<float>::epsilon();
				}

				const int numCellX = (int)floor((spawnConfig->drawEndPos.GetX() - spawnConfig->drawStartPos.GetX()) / offsetX);
				const int numCellZ = (int)floor((spawnConfig->drawEndPos.GetZ() - spawnConfig->drawStartPos.GetZ()) / offsetZ);

				float pos_x, pos_z;
				
				if (numCellX == 0)
				{
					pos_x = spawnConfig->drawStartPos.GetX();
				}
				else
				{
					pos_x = spawnConfig->drawStartPos.GetX() + spawnConfig->posOffset.GetX() * (m_accuSpawnNum % numCellX);
				}

				if (numCellZ == 0)
				{
					pos_z = spawnConfig->drawStartPos.GetZ();
				}
				else
				{
					if (numCellX == 0)
					{
						pos_x = spawnConfig->drawStartPos.GetX() + spawnConfig->posOffset.GetX() * (m_accuSpawnNum % numCellZ);
					}
					else
					{
						pos_z = spawnConfig->drawStartPos.GetZ() + spawnConfig->posOffset.GetZ() * (m_accuSpawnNum / numCellX);
					}
				}

				pos = Vector3(pos_x, 0.0f, pos_z);
			}

			if (m_terrain)
			{
				pos.SetY(m_terrain->GetHeight(pos.GetX(), pos.GetZ()));
			}

			for (int i = 0; i < numGameObj; i++)
			{
				gameObj[i].SetPosition(pos);
			}

			m_accuSpawnNum++;
		}
	}
	else
	{

	}

}

}