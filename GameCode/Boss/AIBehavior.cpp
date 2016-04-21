
// Game include
#include "AIBehavior.h"
#include "Boss.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Math\MathHelper.h"

#include <Windows.h>

void AIBehavior::BossBehavior()
{
	//DE::Vector3 playerPos = DE::Vector3(((Boss*)m_pOwner)->GetPlayerPostion().GetX(), 0.0f, ((Boss*)m_pOwner)->GetPlayerPostion().GetZ());
	//DE::Vector3	bossPos = DE::Vector3(((Boss*)m_pOwner)->GetTransform()->GetPosition().GetX(), 0.0f, ((Boss*)m_pOwner)->GetTransform()->GetPosition().GetZ());
	DE::Vector3 playerPos = ((Boss*)m_pOwner)->GetPlayerPostion();
	DE::Vector3 bossPos = ((Boss*)m_pOwner)->GetPosition();
	
	float distance = (playerPos - bossPos).Length();
	float short_attack_dist = 3.1f;
	float long_attack_dist = 5.0f;
	float speed = 0.8f;
	DE::Vector3 direction = playerPos-bossPos;
	float playerMoveDir = distance - preDist;

	if (!direction.iszero() && ((Boss*)m_pOwner)->GetHP() > 0.0f)
	{
		direction.Normalize();
		DE::Vector3 cross = Cross(m_pOwner->GetTransform()->GetForward().Normal(), direction);
		float dot = cross.Dot(DE::Vector3::UnitY);
		float theta = asinf(cross.Length());
		if (dot < 0.0f)
		{
			theta = 2 * PI - theta;
		}

/*		static wchar_t t[64];
		swprintf(t, 64, L"theta: %f\n", theta);
		OutputDebugStringW(t);*/
			
		if (theta == 0.0f)
		{
			theta += 0.01f;
		}

		if (theta > 0.0f)
		{
			DE::Quaternion quat(DE::Vector3(0, 1, 0), theta);
			DE::Vector3 pos = m_pOwner->GetPosition();
			m_pOwner->TransformBy(quat.GetRotationMatrix());
			m_pOwner->SetPosition(pos);
		}
	

	}

	//direction.Normalize();
	if (!direction.iszero() && distance > 1.4f && ((Boss*)m_pOwner)->GetHP() != 0.0f && ((Boss*)m_pOwner)->GetHP() > 0.0f)
	{
		
		if (((Boss*)m_pOwner)->GetState() == Boss::JUMPATTACKING && ((Boss*)m_pOwner)->GetJumpingTime() < 2.3f)
		{	
			Move(DE::Vector3(0,0,1) *speed*m_fDeltaTime*1.8f);
		}
			
		else if(((Boss*)m_pOwner)->GetState() != Boss::PUNCHING && ((Boss*)m_pOwner)->GetState() != Boss::JUMPATTACKING)
			Move(DE::Vector3(0, 0, 1)*speed*m_fDeltaTime);
	}

	
	if (((Boss*)m_pOwner)->GetHP() <= 0.0f)
	{
		((Boss*)m_pOwner)->SetState(Boss::DYING);
		DE::Handle h(sizeof(Boss_Dying_START_Event));
		new (h) Boss_Dying_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (distance > /*1.2f*/short_attack_dist && ((Boss*)m_pOwner)->GetState() != Boss::JUMPATTACKING && ((Boss*)m_pOwner)->GetState() != Boss::PUNCHING)
	{	
		((Boss*)m_pOwner)->SetState(Boss::WALKING);
		DE::Handle h(sizeof(Boss_Walk_START_Event));
		new (h) Boss_Walk_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);	
	}
	else if (distance > 2.2f && distance <= short_attack_dist && playerMoveDir < 0.0f && ((Boss*)m_pOwner)->GetState() != Boss::JUMPATTACKING && ((Boss*)m_pOwner)->GetState() != Boss::PUNCHING && jumpAttackTime > 5.0f)
	{
		((Boss*)m_pOwner)->SetAttackTime(0.0f);
		((Boss*)m_pOwner)->SetState(Boss::JUMPATTACKING);
		DE::Handle h(sizeof(Boss_Jump_Attack_START_Event));
		new (h) Boss_Jump_Attack_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (distance < 2.2f && distance >= 1.6f && ((Boss*)m_pOwner)->GetState() != Boss::JUMPATTACKING && ((Boss*)m_pOwner)->GetState() != Boss::PUNCHING)
	{
		((Boss*)m_pOwner)->SetState(Boss::WALKING);
		DE::Handle h(sizeof(Boss_Walk_START_Event));
		new (h) Boss_Walk_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (distance < 1.6f && ((Boss*)m_pOwner)->GetState() != Boss::JUMPATTACKING && ((Boss*)m_pOwner)->GetState() != Boss::PUNCHING)// && time > 2.0f)
	{
		((Boss*)m_pOwner)->SetAttackTime(0.0f);
		((Boss*)m_pOwner)->SetState(Boss::PUNCHING);
		DE::Handle h(sizeof(Boss_Punch_START_Event));
		new (h) Boss_Punch_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}

	static wchar_t t[64];
	swprintf(t, 64, L"distance: %f\n", ((Boss*)m_pOwner)->GetJumpingTime());
	OutputDebugStringW(t);
	
	preDist = distance;
}

void AIBehavior::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
	BossBehavior();

	// update the time
	time += deltaTime;
	if (time > 2.1f)
	{
		time = 0.0f;
	}
	jumpAttackTime += deltaTime;
	if (jumpAttackTime > 6.1f)
	{
		jumpAttackTime = 0.0f;
	}
}

void AIBehavior::Move(DE::Vector3 vTrans)
{
	DE::Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}
