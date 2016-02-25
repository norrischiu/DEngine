#include "GameWorld.h"

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
