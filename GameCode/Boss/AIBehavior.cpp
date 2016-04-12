#include <Windows.h>

// Game include
#include "AIBehavior.h"
#include "Boss.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Math\MathHelper.h"


void AIBehavior::HandleMovement()
{
	DE::Vector3 playerPos = ((Boss*)m_pOwner)->GetPlayerPostion();
	float distance = (playerPos - ((Boss*)m_pOwner)->GetTransform()->GetPosition()).Length();
	// temporary using
	float long_attack_dist = 5.0f;
	float short_attack_dist = 1.0f;
	float speed = 0.5f;
	float offset = 0.00001f;
	DE::Vector3 bossPos = ((Boss*)m_pOwner)->GetPosition();
	DE::Vector3 direction = playerPos-bossPos;
	if (!direction.iszero())
	{
		direction.Normalize();
		DE::Vector3 cross = Cross(m_pOwner->GetTransform()->GetForward().Normal(), direction);
		float dot = cross.Dot(DE::Vector3::UnitY);
			float theta = asinf(cross.Length());
			if (dot < 0.0f)
			{
				theta = 2 * PI - theta;
			}
			//static wchar_t s[64];
			//swprintf(s, 64, L"Unbind: %f\n", cross);
			//OutputDebugStringW(s);

			DE::Quaternion quat(DE::Vector3(0, 1, 0), theta);
			m_pOwner->TransformBy(quat.GetRotationMatrix());
		

		
		

	}
//	DE::DEBUG_RENDERER::GetInstance()->DRAW_RAY_SEGMENT(bossPos + DE::Vector3(0, 2, 0), bossPos + direction * 2.0f + DE::Vector3(0, 2, 0));
	DE::DEBUG_RENDERER::GetInstance()->DRAW_RAY_SEGMENT(bossPos, playerPos);

	DE::DEBUG_RENDERER::GetInstance()->DRAW_RAY_SEGMENT(bossPos, bossPos + m_pOwner->GetTransform()->GetForward());
	Move(direction*speed*m_fDeltaTime);
	/*
	if (distance > long_attack_dist)
	{	
		DE::Handle h(sizeof(Boss_Jump_Attack_START_Event));
		new (h) Boss_Jump_Attack_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
		Move(direction);
	}
	else if (distance <= long_attack_dist && distance > short_attack_dist)
	{
		DE::Handle h(sizeof(Boss_Walk_START_Event));
		new (h) Boss_Walk_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (distance <= short_attack_dist)
	{
		DE::Handle h(sizeof(Boss_Punch_START_Event));
		new (h) Boss_Punch_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
*/
	
	
	
}

void AIBehavior::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
	HandleMovement();
}

void AIBehavior::Move(DE::Vector3 vTrans)
{
	DE::Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}
