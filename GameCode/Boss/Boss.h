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

private:

	float							m_fHP;

	Player*							m_Player;
};


#endif // !BOSS_H_
