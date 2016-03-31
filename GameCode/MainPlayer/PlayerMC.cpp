// Game include
#include "PlayerMC.h"
#include "Player.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Event\InputEvent.h"
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Object\CameraComponent.h"
#include "DEngine\Math\simdmath.h"
#include "DEngine\System\Mouse.h"

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
	if (pEvt->m_ID == DE::InputEventID::Mouse_Left_Press_Event)
	{
		DE::Handle h(sizeof(Player_Attack_1_START_Event));
		new (h) Player_Attack_1_START_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
}

void PlayerMC::HandleMouseEvent(DE::Event * pEvt)
{
	if (pEvt->m_ID == DE::InputEventID::Mouse_Move_Event)
	{
		DE::Mouse_Move_Event* pMouseEvt = (DE::Mouse_Move_Event*) pEvt;
		if (pMouseEvt->cursorPosChange[0] != 0 && pMouseEvt->cursorPosChange[1] != 0)
		{
			DE::CameraComponent* cameraComp = ((Player*)m_pOwner)->GetFollowCamera()->GetComponent<DE::CameraComponent>();
			DE::Vector3 oldLookAt = cameraComp->GetLocalLookAt();
			DE::Vector3 newLookAt(pMouseEvt->cursorPosChange[0] / 500.0f, pMouseEvt->cursorPosChange[1] / 500.0f, 0.0f);
			newLookAt = oldLookAt + newLookAt;
			cameraComp->SetLookAt(newLookAt);

			DE::Mouse::ResetWindowCursorPos();
		}
	}
}
