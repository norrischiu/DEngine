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

void GameObjectSpawner::Spawn()
{
	DE::GameObject* gameObject = new GameObject;

	for (auto itr : *m_spawnTarget->getAllComponents())
	{
		gameObject->AddComponent(itr);
	}
}

void GameObjectSpawner::Update(float deltaTime)
{
	m_accuSpawnTime += deltaTime;

	if (m_accuSpawnNum < m_spawnNum)
	{
		if (m_accuSpawnTime > (m_accuSpawnNum + 1) * m_spawnTimeDelay)
		{
			Spawn();

			m_accuSpawnNum++;
		}
	}
	else
	{

	}

}

}