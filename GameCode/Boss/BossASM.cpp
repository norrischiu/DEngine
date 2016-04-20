#include "BossASM.h"
#include "Event\GameEvent.h"
#include "DEngine\Event\EngineEvent.h"
#include "DEngine\Event\EventQueue.h"


BossASM::BossASM(DE::AnimationController * animController)
	: DE::AnimationStateMachine(animController)
{
	AddState("IDLE", "idle");
	SetAsDefaultState("IDLE");
	AddState("WALK", "walk");
	AddState("PUNCH", "punch");
	AddState("JUMP_ATTACK", "jump_attack");
	AddState("DYING", "dying");
	AddState("ROARING", "roaring");
	AddState("SWIPING", "swiping");

	AddTransistion("IDLE", "WALK", 1, 0.5f);
	AddTransistion("WALK", "IDLE", 1, 0.5f);
	AddTransistion("IDLE", "PUNCH", 1, 0.5f);
	AddTransistion("PUNCH", "IDLE", 1, 0.5f);
	AddTransistion("IDLE", "JUMP_ATTACK", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "IDLE", 1, 0.5f);
	AddTransistion("WALK", "PUNCH", 1, 0.5f);
	AddTransistion("PUNCH", "WALK", 1, 0.5f);
	AddTransistion("WALK", "JUMP_ATTACK", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "WALK", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "PUNCH", 1, 0.5f);
	AddTransistion("PUNCH", "JUMP_ATTACK", 1, 0.5f);
	AddTransistion("PUNCH", "DYING", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "DYING", 1, 0.5f);
	AddTransistion("IDLE", "DYING", 1, 0.5f);
	AddTransistion("WALK", "DYING", 1, 0.5f);
	AddTransistion("DYING", "IDLE", 1, 0.5f);
	AddTransistion("PUNCH", "ROARING", 1, 0.5f);
	AddTransistion("ROARING", "PUNCH", 1, 0.5f);
	AddTransistion("ROARING", "IDLE", 1, 0.5f);
	AddTransistion("ROARING", "WALK", 1, 0.5f);
	AddTransistion("ROARING", "JUMP_ATTACK", 1, 0.5f);
	AddTransistion("IDLE", "ROARING", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "ROARING", 1, 0.5f);
	AddTransistion("IDLE", "SWIPING", 1, 0.5f);
	AddTransistion("WALK", "SWIPING", 1, 0.5f);
	AddTransistion("JUMP_ATTACK", "SWIPING", 1, 0.5f);
	AddTransistion("SWIPING", "IDLE", 1, 0.5f);
	AddTransistion("SWIPING", "PUNCH", 1, 0.5f);
	AddTransistion("SWIPING", "WALK", 1, 0.5f);
}

void BossASM::Update(float deltaTime)
{
	DE::AnimationStateMachine::Update(deltaTime);

	int size = DE::EventQueue::GetInstance()->Size(DE::GAME_EVENT);
	for (int i = 0; i < size; ++i)
	{
		DE::Handle hEvt = DE::EventQueue::GetInstance()->Front(DE::GAME_EVENT);
		if (HandleEvent(hEvt))
		{
			DE::EventQueue::GetInstance()->Pop(DE::GAME_EVENT);
		}
		else
		{
			DE::EventQueue::GetInstance()->FrontToBack(DE::GAME_EVENT);
		}
	}
}

bool BossASM::HandleEvent(DE::Handle hEvt)
{
	DE::Event* p_Evt = (DE::Event*) hEvt.Raw();
	switch (p_Evt->m_ID)
	{
	case GameEventID::Boss_Walk_START_Event:
		((Boss*)m_pOwner)->SetState(Boss::WALKING);
		ChangeStateTo("WALK");
		return true;
	case GameEventID::Boss_Walk_END_Event:
		((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("IDLE");
		return true;
	case GameEventID::Boss_Jump_Attack_START_Event:
		((Boss*)m_pOwner)->SetJumpingTime(0.0f);
		((Boss*)m_pOwner)->m_bHitPlayer = false;
		((Boss*)m_pOwner)->SetAttackState(Boss::WAITING);
		ChangeStateTo("JUMP_ATTACK");
		return true;
	case GameEventID::Boss_Jump_Attack_END_Event:
		((Boss*)m_pOwner)->m_bHitPlayer = false;
		((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("ROARING");
		return true;
	case GameEventID::Boss_Punch_START_Event:
		((Boss*)m_pOwner)->m_bHitPlayer = false;
		((Boss*)m_pOwner)->SetState(Boss::PUNCHING);
		((Boss*)m_pOwner)->SetAttackState(Boss::WAITING);
		ChangeStateTo("PUNCH");
		return true;
	case GameEventID::Boss_Punch_END_Event:
		((Boss*)m_pOwner)->m_bHitPlayer = false;
		//((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("ROARING");
		return true;
	case GameEventID::Boss_Dying_START_Event:
		((Boss*)m_pOwner)->SetState(Boss::DYING);
		ChangeStateTo("DYING");
		break;
	case GameEventID::Boss_Dying_END_Event:
		((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("IDLE");
		break;
	case GameEventID::Boss_Roaring_START_Event:
		ChangeStateTo("ROARING");
		break;
	case GameEventID::Boss_Roaring_END_Event:
		((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("IDLE");
		break;
	case GameEventID::Boss_Swiping_START_Event:
		((Boss*)m_pOwner)->SetState(Boss::PUNCHING);
		ChangeStateTo("SWIPING");
		break;
	case GameEventID::Boss_Swiping_END_Event:
		((Boss*)m_pOwner)->SetState(Boss::IDLE);
		ChangeStateTo("IDLE");
		break;
	case DE::EngineEventID::Animation_END_Event:
		if (strcmp(m_pCurrState->m_sName, "PUNCH") == 0)
		{
			ChangeStateTo("ROARING");
			return true;
		}
		else if (strcmp(m_pCurrState->m_sName, "JUMP_ATTACK") == 0)
		{
			ChangeStateTo("ROARING");
			return true;
		}
		else if (strcmp(m_pCurrState->m_sName, "SWIPING") == 0)
		{
			((Boss*)m_pOwner)->SetState(Boss::IDLE);
			ChangeStateTo("IDLE");
			return true;
		}
		else if (strcmp(m_pCurrState->m_sName, "DYING") == 0)
		{
			((Boss*)m_pOwner)->SetState(Boss::IDLE);
			ChangeStateTo("IDLE");
			return true;
		}
		else if (strcmp(m_pCurrState->m_sName, "ROARING") == 0)
		{
			((Boss*)m_pOwner)->SetState(Boss::IDLE);
			ChangeStateTo("IDLE");
			return true;
		}
		return true;
	default:
		return false;
	}
}
