// GameWorld.h:
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "GameObject.h"
#include "Utilities\MyArray.h"

namespace DE
{

class GameWorld
{
public:
	GameWorld() 
		: m_GameObjectList(0)
	{}

	static GameWorld*					GetInstance();

	void Update(float deltaTime);

	void AddGameObject(GameObject* gameObj);

	GameObject* GetGameObjectAt(unsigned int index);

	inline int GetGameObjectNum()
	{
		return m_GameObjectList.Size();
	}

	bool RayCast(Vector3 start, Vector3 end);

	bool ShapeCast();

	// Destructor
	~GameWorld()
	{
		m_GameObjectList.Clear();
	}

private:
	static GameWorld*					m_pGameWorld;

	MyArray<GameObject*>				m_GameObjectList;
};

};

#endif