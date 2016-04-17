// Player.h: main player in this game
#ifndef PLAYER_H_
#define PLAYER_H_

// Engine include
#include "DEngine\GameObject\GameObject.h"

class Player : public DE::GameObject
{

public:

	Player();

	// Inherited via GameObject
	void Update(float deltaTime) override;

	void UpdateSpawner(float deltaTime);

	void Clone(const int spawnNum, const DE::Vector3& spawnPos, const float spawnTimeDelay);

	//~Player();

public:

	GameObject*				m_Weapon;

	float					m_fHP;

};


#endif // !PLAYER_H_
