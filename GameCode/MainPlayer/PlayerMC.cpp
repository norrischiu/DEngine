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

bool PlayerMC::HandleKeyboardEvent(DE::Event * pEvt)
{
	if (DE::MovementController::HandleKeyboardEvent(pEvt))
	{
		return true;
	}
	// catch unused keyboard event
	else if (pEvt->m_ID == DE::InputEventID::Key_W_Press_Event
		|| pEvt->m_ID == DE::InputEventID::Key_A_Press_Event
		|| pEvt->m_ID == DE::InputEventID::Key_S_Press_Event
		|| pEvt->m_ID == DE::InputEventID::Key_D_Press_Event
		|| pEvt->m_ID == DE::InputEventID::Key_W_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_A_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_S_Release_Event
		|| pEvt->m_ID == DE::InputEventID::Key_D_Release_Event)
	{
		return true;
	}
	return false;
}

bool PlayerMC::HandleMouseEvent(DE::Event * pEvt)
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
			*camera->GetLocalTransform() = rot;
			camera->GetLocalTransform()->SetPosition(offset);

			DE::Mouse::ResetWindowCursorPos();
		}
		return true;
	}
	else if (pEvt->m_ID == DE::InputEventID::Mouse_Left_Press_Event)
	{
		if (((Player*)m_pOwner)->GetState() == Player::IDLING_MOVING)
		{
			((Player*)m_pOwner)->SetState(Player::ATTACKING);
			DE::Handle h(sizeof(Player_Attack_1_START_Event));
			new (h) Player_Attack_1_START_Event;
			DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
			m_ComboSequence[0] = true;
		}
		else if (m_ComboSequence[0] && m_fComboTime > m_ComboDelays[0])
		{
			DE::Handle h(sizeof(Player_Attack_2_START_Event));
			new (h) Player_Attack_2_START_Event;
			DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
			m_ComboSequence[0] = false;
			m_ComboSequence[1] = true;
			m_fComboTime = 0.0f;
		}
		else if (m_ComboSequence[1] && m_fComboTime > m_ComboDelays[1])
		{
			DE::Handle h(sizeof(Player_Attack_3_START_Event));
			new (h) Player_Attack_3_START_Event;
			DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
			m_ComboSequence[1] = false;
			m_ComboSequence[2] = true;
			m_fComboTime = 0.0f;
		}
		return true;
	}
	else if (pEvt->m_ID == DE::InputEventID::Mouse_Right_Press_Event && ((Player*)m_pOwner)->GetState() == Player::IDLING_MOVING)
	{
		m_bDodge = true;
		return true;
	}
	else
	{
		if (pEvt->m_ID == DE::InputEventID::Mouse_Left_Hold_Event)
		{
			return true;
		}
	}
	return false;
}

void PlayerMC::Dispatch()
{
	if (m_vTrans.iszero() && ((Player*)m_pOwner)->GetState() == Player::IDLING_MOVING)
	{
		DE::Handle h(sizeof(Player_Walk_END_Event));
		new (h) Player_Walk_END_Event;
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	else if (m_bDodge && !m_vTrans.iszero())
	{
		m_vDodgeDir = m_vTrans.Normal();

		((Player*)m_pOwner)->SetState(Player::DOGDING);
		DE::Handle h(sizeof(Player_Dodge_START_Event));
		new (h) Player_Dodge_START_Event;
		((Player_Dodge_START_Event*)h.Raw())->m_vDir = m_vTrans.Normal();
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
		m_bDodge = false;
	}
	else if (!m_vTrans.iszero() && ((Player*)m_pOwner)->GetState() == Player::IDLING_MOVING)
	{
		Move(m_vTrans);

		DE::Handle h(sizeof(Player_Walk_PLAYING_Event));
		new (h) Player_Walk_PLAYING_Event;
		((Player_Walk_PLAYING_Event*)h.Raw())->m_vDir = m_vTrans.Normal();
		DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
	}
	if (((Player*)m_pOwner)->GetState() == Player::DOGDING)
	{
		Move(m_vDodgeDir * m_fSpeed * m_fDeltaTime * 3.0f);
	}
	if (m_ComboSequence[0] || m_ComboSequence[1])
	{
		m_fComboTime += m_fDeltaTime;
	}
	if (m_ComboSequence[2])
	{
		Move(m_pOwner->GetTransform()->GetForward().Normal() * m_fSpeed * m_fDeltaTime * 0.5f);
	}
	m_vTrans = DE::Vector3::Zero;
}
