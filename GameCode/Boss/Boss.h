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
		// normal state
		WAITING, PUNCHING, JUMPATTACKING, IDLE, DYING,

		// attacking state
		WALKING, ATTACKED
	};

	Boss(Player* player);

	// Inherited via GameObject
	void Update(float deltaTime) override;

	~Boss();

	DE::Vector3 GetPlayerPostion()
	{
		return m_pPlayer->GetPosition();
		//return m_Player->GetTransform()->GetPosition();
	}

	void SetPlayer(Player* player)
	{
		m_pPlayer = player;
	}

	void SetState(eState state) 
	{
		m_eState = state;
	}

	eState GetState()
	{
		return m_eState;
	}

	void SetAttackState(eState state)
	{
		m_eAttacked = state;
	}

	eState GetAttackState()
	{
		return m_eAttacked;
	}

	float GetHP()
	{
		return m_fHP;
	}

	void SetAttackTime(float attactTime)
	{
		m_fAttackTime = attactTime;
	}

	void SetJumpingTime(float jumpingTime)
	{
		m_fJumpingTime = jumpingTime;
	}

	float GetJumpingTime()
	{
		return m_fJumpingTime;
	}

	GameObject*	GetBlood()
	{
		return m_pBlood;
	}

	Player*		GetPlayer()
	{
		return m_pPlayer;
	}

	bool							m_bHitPlayer;

private:

	float							m_fHP;

	float							m_fAttackTime;

	float							m_fJumpingTime;

	Player*							m_pPlayer;

	eState							m_eState;

	eState							m_eAttacked;

	DE::GameObject*					m_pLeftHand;

	DE::GameObject*					m_pRightHand;

	DE::GameObject*					m_pBlood;
};


#endif // !BOSS_H_
