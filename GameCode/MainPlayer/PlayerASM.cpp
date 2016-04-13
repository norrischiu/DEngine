// PlayerASM.cpp

// Game include
#include "PlayerASM.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Event\EngineEvent.h"
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Memory\Handle.h"

PlayerASM::PlayerASM(DE::AnimationController * animController)
	: DE::AnimationStateMachine(animController)
{
	AddState("WALK", "walk");
	SetAsDefaultState("WALK");
}

void PlayerASM::Update(float deltaTime)
{
	DE::AnimationStateMachine::Update(deltaTime);

	/*while (!DE::EventQueue::GetInstance()->Empty(DE::GAME_EVENT))
	{
		DE::Handle hEvt = DE::EventQueue::GetInstance()->Front(DE::GAME_EVENT);
		HandleEvent(hEvt);
		DE::EventQueue::GetInstance()->Pop(DE::GAME_EVENT);
	}*/
}

void PlayerASM::HandleEvent(DE::Handle hEvt)
{
	/*DE::Event* pEvt = (DE::Event*) hEvt.Raw();
	switch (pEvt->m_ID)
	{
		case GameEventID::Player_Walk_START_Event:
			ChangeStateTo("WALK");
			break;
		case GameEventID::Player_Walk_END_Event:
			ChangeStateTo("IDLE");
			break;	
		case GameEventID::Player_Attack_1_START_Event:
			ChangeStateTo("ATTACK");
			break;
		case GameEventID::Player_Attack_1_END_Event:
			ChangeStateTo("IDLE");
			break;
		case DE::EngineEventID::Animation_END_Event:
			if (strcmp(m_pCurrState->m_sName, "ATTACK") == 0)
			{
				DE::Handle hEvt(sizeof(Player_Attack_1_END_Event));
				new (hEvt) Player_Attack_1_END_Event;
				DE::EventQueue::GetInstance()->Add(hEvt, DE::GAME_EVENT);
			}
			break;
	}*/
}
