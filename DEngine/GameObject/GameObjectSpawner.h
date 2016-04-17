#pragma once

#include "GameObject.h"
#include "../Graphics/VertexFormat.h"

namespace DE
{

class GameObjectSpawner
{
public:
	GameObjectSpawner(GameObject* spawnTarget, const int spawnNum, const Vector3& spawnPos, const float spawnTimeDelay);
	~GameObjectSpawner();

	virtual void Update(float deltaTime);

	GameObject* m_spawnTarget;
	int m_spawnNum;
	int m_accuSpawnNum;
	float m_spawnTimeDelay;
	float m_accuSpawnTime;
	Vector3 m_spawnPos;
};

}