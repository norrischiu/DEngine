// PlayerASM.cpp
#include "PlayerASM.h"

// Game include
#include "Event\GameEvent.h"
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Memory\Handle.h"

// Engine include

PlayerASM::PlayerASM(AnimationController * animController)
	: DE::AnimationStateMachine(animController)
{
	AddState("IDLE", "idle");
	SetAsDefaultState("IDLE");
	AddState("ATTACK", "attack1");
	AddTransistion("IDLE", "ATTACK", 0);
	AddTransistion("ATTACK", "IDLE", 0);
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
		case EventID::Player_Attack_1_START_Event:
			ChangeStateTo("ATTACK");
			break;
		case EventID::Player_Attack_1_END_Event:
			ChangeStateTo("IDLE");
			break;
	}
}
