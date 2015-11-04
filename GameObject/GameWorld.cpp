#include "GameWorld.h"

GameWorld* GameWorld::m_pGameWorld;

GameWorld * GameWorld::GetInstance()
{
	if (!m_pGameWorld)
		m_pGameWorld = new GameWorld();
	return m_pGameWorld;
}

std::vector<GameObject*>& GameWorld::GetGameObjectList()
{
	return m_GameObjectList;
}
