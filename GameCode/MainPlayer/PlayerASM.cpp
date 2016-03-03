// PlayerASM.cpp

// Game include
#include "PlayerASM.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Event\EngineEvent.h"
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Memory\Handle.h"

#include <vector>

PlayerASM::PlayerASM(AnimationController * animController)
	: DE::AnimationStateMachine(animController)
{
	AddState("IDLE", "idle");
	SetAsDefaultState("IDLE");
	AddState("WALK", "walk");
	AddState("ATTACK", "attack1");

	AddTransistion("IDLE", "WALK", 1, 0.5f);
	AddTransistion("WALK", "IDLE", 1, 0.5f);
	AddTransistion("IDLE", "ATTACK", 1, 0.5f);
	AddTransistion("ATTACK", "IDLE", 1, 0.5f);
	AddTransistion("WALK", "ATTACK", 1, 0.5f);
	AddTransistion("ATTACK", "WALK", 1, 0.5f);
}

void PlayerASM::Update(float deltaTime)
{
	DE::AnimationStateMachine::Update(deltaTime);

	while (!EventQueue::GetInstance()->Empty(GAME_EVENT))
	{
		Handle hEvt = EventQueue::GetInstance()->Front(GAME_EVENT);
		HandleEvent(hEvt);
		EventQueue::GetInstance()->Pop(GAME_EVENT);
	}
}

void PlayerASM::HandleEvent(Handle hEvt)
{
	Event* pEvt = (Event*) hEvt.Raw();
	switch (pEvt->m_ID)
	{
		case GameEventID::Player_Walk_START_Event:
			ChangeStateTo("WALK");
			break;
		case GameEventID::Player_Walk_END_Event:
			ChangeStateTo("IDLE");
			break;		
//		case DE::EngineEventID::Animation_END_Event:
//			ChangeStateTo("IDLE");
//			break;
	}
}
