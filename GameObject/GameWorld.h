
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

	void Update(float deltaTime);

	void AddGameObject(GameObject* gameObj);
	
	GameObject* GetGameObjectAt(unsigned int index);

	// Destructor
	~GameWorld()
	{
		for (auto itr : m_GameObjectList)
		{
			delete itr;
		}
	}

private:
	static GameWorld*					m_pGameWorld;

	std::vector<GameObject*>			m_GameObjectList;
};


#endif