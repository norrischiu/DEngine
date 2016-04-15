// Boss.h: main enemy in this game
#ifndef BOSS_H_
#define BOSS_H_

// Game include
#include "GameCode\MainPlayer\Player.h"

// Engine include
#include "DEngine\GameObject\GameObject.h"
#include "DEngine\Object\Camera.h"
#include "DEngine\Math\simdmath.h"

class Boss : public DE::GameObject
{

	friend class Player;

public:

	enum eState
	{
		WAITING, PUNCHING, JUMPATTACKING, IDLE, DYING
	};

	Boss(Player* player);

	// Inherited via GameObject
	void Update(float deltaTime) override;

	~Boss();

	DE::Vector3 GetPlayerPostion()
	{
		return m_Player->GetPosition();
		//return m_Player->GetTransform()->GetPosition();
	}

	void SetPlayer(Player* player)
	{
		m_Player = player;
	}

	void SetState(eState state) 
	{
		m_eState = state;
	}

	eState GetState()
	{
		return m_eState;
	}

	float GetHP()
	{
		return m_fHP;
	}

	bool							m_bHitPlayer;

private:

	float							m_fHP;

	Player*							m_Player;

	eState							m_eState;

	DE::GameObject*					m_pLeftHand;

	DE::GameObject*					m_pRightHand;
};


#endif // !BOSS_H_
