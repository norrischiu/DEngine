#pragma once
#include "DEngine\GameObject\GameObjectSpawner.h"
#include "DEngine\Graphics\VertexFormat.h"
#include "DEngine\GameObject\SpawnConfig.h"
#include "Player.h"

class PlayerGOS : public DE::GameObjectSpawner
{
public:
	PlayerGOS(DE::SpawnConfig* spawnConfig, DE::SpawnConfigType spawnConfigType, DE::Terrain* terrain = nullptr);
	~PlayerGOS();

	int Spawn(DE::GameObject*& gameObj) override;

private:
	void CopyOverrideComponent(DE::GameObject* gameObj, DE::Component* component);
	void CopyComponent(DE::GameObject* spawnTarget, DE::GameObject* gameObj, const int componentID);
};

