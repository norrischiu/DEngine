// Player.h: main player in this game
#ifndef PLAYER_H_
#define PLAYER_H_

// Engine include
#include "DEngine\GameObject\GameObject.h"
#include "ThirdPersonCamera.h"

class Player : public DE::GameObject
{

	friend class Boss;

public:

	enum eState
	{
		ATTACKING, IDLING_MOVING, DOGDING
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

	inline void SetBoss(Boss* pBoss)
	{
		m_pBoss = pBoss;
	}

	inline Boss* GetBoss()
	{
		return m_pBoss;
	}

	bool								m_bHitBoss;

private:

	DE::GameObject*						m_Weapon;

	ThirdPersonCamera*					m_FollowCamera;

	// Health point
	float								m_fHP;

	// Player action state
	eState								m_eState;

	Boss*								m_pBoss;

};


#endif // !PLAYER_H_
