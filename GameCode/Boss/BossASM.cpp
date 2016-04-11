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
		ChangeStateTo("WALK");
		return true;
	case GameEventID::Boss_Walk_END_Event:
		ChangeStateTo("IDLE");
		return true;
	case GameEventID::Boss_Jump_Attack_START_Event:
		ChangeStateTo("JUMP_ATTACK");
		return true;
	case GameEventID::Boss_Jump_Attack_END_Event:
		ChangeStateTo("IDLE");
		return true;
	case GameEventID::Boss_Punch_START_Event:
		ChangeStateTo("PUNCH");
		return true;
	case GameEventID::Boss_Punch_END_Event:
		ChangeStateTo("IDLE");
		return true;
	case DE::EngineEventID::Animation_END_Event:
		if (strcmp(m_pCurrState->m_sName, "PUNCH") == 0)
		{
			DE::Handle hEvt(sizeof(Boss_Punch_END_Event));
			new (hEvt) Boss_Punch_END_Event;
			DE::EventQueue::GetInstance()->Add(hEvt, DE::GAME_EVENT);
		}
		else if (strcmp(m_pCurrState->m_sName, "JUMP_ATTACK") == 0)
		{
			DE::Handle hEvt(sizeof(Boss_Jump_Attack_END_Event));
			new (hEvt) Boss_Jump_Attack_END_Event;
			DE::EventQueue::GetInstance()->Add(hEvt, DE::GAME_EVENT);
		}
		return true;
	default:
		return false;
	}
}
