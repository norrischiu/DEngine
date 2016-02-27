// Game include
#include "PlayerMC.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Event\InputEvent.h"
#include "DEngine\Event\EventQueue.h"

void PlayerMC::HandleKeyboardEvent(Event * pEvt)
{
	DE::MovementController::HandleKeyboardEvent(pEvt);
	if (pEvt->m_ID == DE::EventID::Key_W_Press_Event)
	{
		Handle h(sizeof(Player_Walk_START_Event));
		new (h) Player_Walk_START_Event;
		EventQueue::GetInstance()->Add(h, GAME_EVENT);
	}
}

void PlayerMC::HandleMouseEvent(Event * pEvt)
{
	DE::MovementController::HandleMouseEvent(pEvt);
}
