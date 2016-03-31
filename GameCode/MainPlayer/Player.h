// Player.h: main player in this game
#ifndef PLAYER_H_
#define PLAYER_H_

// Engine include
#include "DEngine\GameObject\GameObject.h"
#include "DEngine\Object\Camera.h"

class Player : public DE::GameObject
{

public:

	Player();

	// Inherited via GameObject
	void Update(float deltaTime) override;

	//~Player();

	inline DE::GameObject* GetWeapon()
	{
		return m_Weapon;
	}

	inline DE::Camera* GetFollowCamera()
	{
		return m_FollowCamera;
	}

private:

	DE::GameObject*					m_Weapon;

	DE::Camera*						m_FollowCamera;

	float							m_fHP;

};


#endif // !PLAYER_H_
