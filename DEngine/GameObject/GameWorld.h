// GameWorld.h:
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <vector>
#include "GameObject.h";

namespace DE
{

class GameWorld
{
public:
	GameWorld() {}

	static GameWorld*					GetInstance();

	void Update(float deltaTime);

	void AddGameObject(GameObject* gameObj);

	GameObject* GetGameObjectAt(unsigned int index);

	inline int GetGameObjectNum()
	{
		return m_GameObjectList.size();
	}

	bool RayCast(Vector3 start, Vector3 end);

	bool ShapeCast();

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

};

#endif