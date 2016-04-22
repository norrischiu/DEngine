#include "SpawnManager.h"

namespace DE
{

SpawnManager* SpawnManager::m_pInstance = nullptr;

SpawnManager::SpawnManager() : m_spawnState(SpawnState::SPAWN_INACTIVE)
{
}


SpawnManager::~SpawnManager()
{
}

SpawnManager* SpawnManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SpawnManager;
	}

	return m_pInstance;
}

SpawnManager::SpawnState SpawnManager::GetSpawnState()
{
	return m_spawnState;
}

void SpawnManager::AddSpawner(GameObjectSpawner* gameObjSpawner)
{
	m_vGameObjSpawnerList.push_back(gameObjSpawner);
}

void SpawnManager::Update(float elapsedTime)
{
	for (auto itr = m_vGameObjSpawnerList.begin(); itr != m_vGameObjSpawnerList.end();)
	{
		if (!(*itr)->IsSpawnFinish())
		{
			m_spawnState = SpawnState::SPAWN_IN_PROGRESS;
			(*itr)->Update(elapsedTime);
			itr++;
		}
		else
		{
			delete *itr;
			itr = m_vGameObjSpawnerList.erase(itr);

			if (itr == m_vGameObjSpawnerList.end())
			{
				m_spawnState = SpawnState::SPAWN_FINISHED;
			}
		}
	}
}

}