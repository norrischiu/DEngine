// PlayerASM.cpp
#include "PlayerASM.h"

// Game include
#include "Event\GameEvent.h"
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Event\EventQueue.h"

// Engine include

void PlayerASM::Update(float deltaTime)
{
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
			m_pController->setActiveAnimationSet("idle", false);
			m_pController->setActiveAnimationSet("attack1", true);
			break;
		case EventID::Player_Attack_1_END_Event:
			m_pController->setActiveAnimationSet("attack1", false);
			m_pController->setActiveAnimationSet("idle", true);
			break;
	}
}
