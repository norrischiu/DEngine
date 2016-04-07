// Player.h: main player in this game
#ifndef PLAYER_H_
#define PLAYER_H_

// Engine include
#include "DEngine\GameObject\GameObject.h"
#include "ThirdPersonCamera.h"

class Player : public DE::GameObject
{

public:

	enum eState
	{
		ATTACKING, NOT_ATTACKING
	};

	Player();

	// Inherited via GameObject
	void Update(float deltaTime) override;

	//~Player();

	inline DE::GameObject* GetWeapon()
	{
		return m_Weapon;
	}

	inline ThirdPersonCamera* GetFollowCamera()
	{
		return m_FollowCamera;
	}

	inline void SetState(eState state)
	{
		m_eState = state;
	}

	inline eState GetState()
	{
		return m_eState;
	}

private:

	DE::GameObject*						m_Weapon;

	ThirdPersonCamera*					m_FollowCamera;

	// Health point
	float								m_fHP;

	eState								m_eState;

};


#endif // !PLAYER_H_
