#include "MovementController.h"
#include "Graphics\D3D11Renderer.h"
#include "Event\InputEvent.h"
#include "GameObject\GameObject.h"

namespace DE
{

void MovementController::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
	while (!EventQueue::GetInstance()->Empty(INPUT_EVENT))
	{
		Handle hEvt = EventQueue::GetInstance()->Front(INPUT_EVENT);
		Event* pEvt = (Event*)hEvt.Raw();
		HandleKeyboardEvent(pEvt);
		HandleMouseEvent(pEvt);
		EventQueue::GetInstance()->Pop(INPUT_EVENT);
	}
}

void MovementController::HandleKeyboardEvent(Event* pEvt)
{
	if (pEvt->m_ID == InputEventID::Key_W_Hold_Event)
	{
		Vector3 vForward = DE::Vector3::UnitZ;
		Move(vForward * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == InputEventID::Key_S_Hold_Event)
	{
		Vector3 vBackward = DE::Vector3::NegativeUnitZ;
		Move(vBackward * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == InputEventID::Key_D_Hold_Event)
	{
		Vector3 vRight = Vector3::UnitX;
		Move(vRight * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == InputEventID::Key_A_Hold_Event)
	{
		Vector3 vLeft = Vector3::NegativeUnitX;
		Move(vLeft * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == InputEventID::Key_W_Press_Event)
	{
		//Vector3 vForward = m_pOwner->GetTransform().GetForward();
		//Move(vForward * m_fDeltaTime * m_fSpeed);
	}
}

void MovementController::HandleMouseEvent(Event* pEvt)
{
	if (pEvt->m_ID == InputEventID::Mouse_Move_Event)
	{
		Mouse_Move_Event* pMouseEvt = (Mouse_Move_Event*) pEvt;
		if (pMouseEvt->cursorPosChange[0] != 0 && pMouseEvt->cursorPosChange[1] != 0)
		{
			Matrix4 rot;
			rot.CreateRotationX(pMouseEvt->cursorPosChange[0] / 150.0f);
			//m_pOwner->Transform(rot);
		}
		//D3D11Renderer::GetInstance()->GetCamera()->rotateVPos(pMouseEvt->cursorPosChange[0] / 150.0f, pMouseEvt->cursorPosChange[1] / 150.0f);
	}
}

void MovementController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}

};