#pragma once
#include "DEngine\GameObject\GameObjectSpawner.h"
#include "DEngine\Graphics\VertexFormat.h"
#include "Player.h"

class PlayerGOS : public DE::GameObjectSpawner
{
public:
	PlayerGOS(Player* player, const int spawnNum, const DE::Vector3& spawnPos, const float spawnTimeDelay);
	~PlayerGOS();

	// Inherited via AnimationStateMachine
	void Update(float deltaTime) override;

private:
	DE::Vector3 weaponSpawnPos;
};

