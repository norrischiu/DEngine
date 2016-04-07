// Game include
#include "PlayerMC.h"
#include "Player.h"
#include "ThirdPersonCamera.h"
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

	if (pEvt->m_ID == DE::InputEventID::Key_W_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_A_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_S_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_D_Release_Event)
	{
		DE::Handle h(sizeof(Player_Walk_END_Event));
		new (h) Player_Walk_END_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (pEvt->m_ID == DE::InputEventID::Mouse_Left_Press_Event)
	{
		((Player*)m_pOwner)->SetState(Player::ATTACKING);

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
			ThirdPersonCamera* camera = ((Player*)m_pOwner)->GetFollowCamera();
			camera->m_fYaw += atanf(pMouseEvt->cursorPosChange[0] / 500.0f);
			camera->m_fPitch += atanf(pMouseEvt->cursorPosChange[1] / 500.0f);

			DE::Vector3 offset = camera->m_vOffset;
			DE::Matrix4 rot;
			rot.CreateRotationY(camera->m_fYaw);
			offset.Transform(rot);
			//rot.CreateRotationX(camera->m_fPitch);
			//offset.Transform(rot);
			((Player*)m_pOwner)->GetFollowCamera()->GetComponent<DE::CameraComponent>()->SetPosition(offset);

			DE::Mouse::ResetWindowCursorPos();
		}
	}
}

void PlayerMC::Dispatch()
{
	if (((Player*)m_pOwner)->GetState() != Player::ATTACKING)
	{
		if (!m_vTrans.iszero())
		{
			Move(m_vTrans);
			DE::Handle h(sizeof(Player_Walk_START_Event));
			new (h) Player_Walk_START_Event;
			((Player_Walk_START_Event*)h.Raw())->m_vDir = m_vTrans.Normalize();
			DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
		}
	}
	m_vTrans = DE::Vector3::Zero;
}
