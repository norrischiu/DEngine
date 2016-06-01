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
	const unsigned int size = m_GameObjectList.Size();
	for (int i = 0; i < size; ++i)
	{
		m_GameObjectList[i]->Update(deltaTime);
	}
}

void GameWorld::AddGameObject(GameObject* gameObj)
{
	m_GameObjectList.Add(gameObj);
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
	const unsigned int size = m_GameObjectList.Size();
	for (int i = 0; i < size; ++i)
	{
		if (m_GameObjectList[i]->GetComponent<Body>())
		{
			query.collision(&ray, m_GameObjectList[i]->GetComponent<Body>());
			if (query.getCollide() && abs(query.getDistance()) <= dist)
			{
				return true;
			}
		}
	}
	return false;
}

};