#include "MovementController.h"
#include "Graphics\D3D11Renderer.h"
#include "Event\InputEvent.h"
#include "GameObject\GameObject.h"

namespace DE
{

void MovementController::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
	int size = EventQueue::GetInstance()->Size(INPUT_EVENT);
	for (int i = 0; i < size; ++i)
	{
		Handle hEvt = EventQueue::GetInstance()->Front(INPUT_EVENT);
		Event* pEvt = (Event*)hEvt.Raw();
		if (HandleKeyboardEvent(pEvt) || HandleMouseEvent(pEvt))
		{
			EventQueue::GetInstance()->Pop(INPUT_EVENT);
		}
		else
		{
			EventQueue::GetInstance()->FrontToBack(INPUT_EVENT);
		}
	}
	Dispatch();
}

void MovementController::Dispatch()
{
	Move(m_vTrans);
	m_vTrans = Vector3::Zero;
}

bool MovementController::HandleKeyboardEvent(Event* pEvt)
{
	if (pEvt->m_ID == InputEventID::Key_W_Hold_Event)
	{
		Vector3 vForward = DE::Vector3::UnitZ;
		m_vTrans += (vForward * m_fDeltaTime * m_fSpeed);
		return true;
	}
	else if (pEvt->m_ID == InputEventID::Key_S_Hold_Event)
	{
		Vector3 vBackward = DE::Vector3::NegativeUnitZ;
		m_vTrans += (vBackward * m_fDeltaTime * m_fSpeed);
		return true;
	}
	else if (pEvt->m_ID == InputEventID::Key_D_Hold_Event)
	{
		Vector3 vRight = DE::Vector3::UnitX;
		m_vTrans += (vRight * m_fDeltaTime * m_fSpeed);
		return true;
	}
	else if (pEvt->m_ID == InputEventID::Key_A_Hold_Event)
	{
		Vector3 vLeft = DE::Vector3::NegativeUnitX;
		m_vTrans += (vLeft * m_fDeltaTime * m_fSpeed);
		return true;
	}
	return false;
}

bool MovementController::HandleMouseEvent(Event* pEvt)
{
	if (pEvt->m_ID == InputEventID::Mouse_Move_Event)
	{
		Mouse_Move_Event* pMouseEvt = (Mouse_Move_Event*) pEvt;
		if (pMouseEvt->cursorPosChange[0] != 0 && pMouseEvt->cursorPosChange[1] != 0)
		{
			//Matrix4 rot;
			//rot.CreateRotationX(pMouseEvt->cursorPosChange[0] / 150.0f);
			//m_pOwner->Transform(rot);
		}
		return true;
	}
	return false;
}

void MovementController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}

};