
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <vector>
#include "GameObject.h"
class GameObject;

class GameWorld
{
public:
	GameWorld(){}

	static GameWorld*					GetInstance();

	std::vector<GameObject*>&	GetGameObjectList();

private:
	static GameWorld*					m_pGameWorld;

	std::vector<GameObject*>		m_GameObjectList;
};


#endif