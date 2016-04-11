#include "GameWorld.h"
#include "Physics\cdRay.h"
#include "Physics\cdCollide.h"

namespace DE
{

GameWorld* GameWorld::m_pGameWorld;

GameWorld * GameWorld::GetInstance()
{
	if (!m_pGameWorld)
		m_pGameWorld = new GameWorld();
	return m_pGameWorld;
}

void GameWorld::Update(float deltaTime)
{
	for (auto itr : m_GameObjectList)
	{
		itr->Update(deltaTime);
	}
}

void GameWorld::AddGameObject(GameObject* gameObj)
{
	m_GameObjectList.push_back(gameObj);
}

GameObject* GameWorld::GetGameObjectAt(unsigned int index)
{
	return m_GameObjectList[index];
}

bool GameWorld::RayCast(Vector3 start, Vector3 end)
{
	Ray ray((end - start).Normalize(), start);
	float dist = (end - start).Length();
	Collide query;
	for (GameObject* itr : m_GameObjectList)
	{
		if (itr->GetComponent<Body>())
		{
			query.collision(&ray, itr->GetComponent<Body>());
			if (query.getCollide() && abs(query.getDistance()) <= dist)
			{
				return true;
			}
		}
	}
	return false;
}

};