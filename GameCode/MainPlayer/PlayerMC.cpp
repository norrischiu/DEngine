// Game include
#include "PlayerMC.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Event\InputEvent.h"
#include "DEngine\Event\EventQueue.h"

void PlayerMC::HandleKeyboardEvent(DE::Event * pEvt)
{
	DE::MovementController::HandleKeyboardEvent(pEvt);
	if (pEvt->m_ID == DE::InputEventID::Key_W_Press_Event)
	{
		DE::Handle h(sizeof(Player_Walk_START_Event));
		new (h) Player_Walk_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	if (pEvt->m_ID == DE::InputEventID::Key_W_Release_Event)
	{
		DE::Handle h(sizeof(Player_Walk_END_Event));
		new (h) Player_Walk_END_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
}

void PlayerMC::HandleMouseEvent(DE::Event * pEvt)
{
	DE::MovementController::HandleMouseEvent(pEvt);
}
