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

private:

	float					m_fHP;

};


#endif // !PLAYER_H_
