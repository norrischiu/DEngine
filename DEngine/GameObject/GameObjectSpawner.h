#pragma once

#include "GameObject.h"
#include "SpawnConfig.h"
#include "../Graphics/Terrain/Terrain.h"

namespace DE
{

class GameObjectSpawner
{

public:
	GameObjectSpawner(SpawnConfig* spawnConfig, SpawnConfigType spawnConfigType, Terrain* terrain);
	~GameObjectSpawner();

	void AddOverrideComponent(Component* component);
	int FindOverrideComponent(int componentID);
	bool IsSpawnFinish();

	void Update(float deltaTime);
	virtual int Spawn(GameObject*& gameObj);

public:
	SpawnConfig* m_spawnConfig;
	SpawnConfigType m_spawnConfigType;
	std::vector<Component*> m_vOverrideComponents;

private:
	Terrain* m_terrain;
	float m_accuSpawnTime;
	int m_accuSpawnNum;
	bool m_spawnActive;
};

}