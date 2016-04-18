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
		ATTACKING, LOCOMOTION, DOGDING, IMPACTING
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

	inline float GetStamina()
	{
		return m_fStamina;
	}

	inline void AddStamina(int diff)
	{
		m_fStamina += diff;
	}

	bool								m_bHitBoss;

private:

	DE::GameObject*						m_Weapon;

	ThirdPersonCamera*					m_FollowCamera;

	// Health point
	float								m_fHP;

	// Stamina point
	float								m_fStamina;

	// Player action state
	eState								m_eState;

	// Point to friend class boss
	Boss*								m_pBoss;

};


#endif // !PLAYER_H_
