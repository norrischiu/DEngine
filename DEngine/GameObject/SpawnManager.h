#pragma once
#include <vector>
#include "GameObjectSpawner.h"

namespace DE
{

class SpawnManager
{
public:
	enum SpawnState
	{
		SPAWN_INACTIVE,
		SPAWN_IN_PROGRESS,
		SPAWN_FINISHED
	};

	SpawnManager();
	~SpawnManager();

	SpawnManager::SpawnState GetSpawnState();
	void AddSpawner(GameObjectSpawner* gameObjSpawner);
	static SpawnManager* GetInstance();

	void Update(float elapsedTime);

private:
	static SpawnManager* m_pInstance;
	std::vector<GameObjectSpawner*> m_vGameObjSpawnerList;
	SpawnManager::SpawnState m_spawnState;
};

};