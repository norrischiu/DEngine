#include "MovementController.h"
#include "Graphics\D3D11Renderer.h"
#include "Event\InputEvent.h"
#include "GameObject\GameObject.h"

void MovementController::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
	Event* pEvt = EventQueue::GetInstance()->Front();
	while (pEvt != nullptr)
	{
		HandleKeyboardEvent(pEvt);
		HandleMouseEvent(pEvt);
		EventQueue::GetInstance()->Pop();
		pEvt = EventQueue::GetInstance()->Front();
	}
}

void MovementController::HandleKeyboardEvent(Event* pEvt)
{
	if (pEvt->m_ID == EventID::Key_W_Hold_Event)
	{
		Vector3 vForward = Vector3::UnitZ; // temp
		Move(vForward * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == EventID::Key_S_Hold_Event)
	{
		Vector3 vBackward = Vector3::NegativeUnitZ;
		Move(vBackward * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == EventID::Key_D_Hold_Event)
	{
		Vector3 vRight = Vector3::UnitX;
		Move(vRight * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == EventID::Key_A_Hold_Event)
	{
		Vector3 vLeft = Vector3::NegativeUnitX;
		Move(vLeft * m_fDeltaTime * m_fSpeed);
	}
	else if (pEvt->m_ID == EventID::Key_W_Press_Event)
	{
		Vector3 vForward = Vector3::UnitZ; // temp
		Move(vForward * m_fDeltaTime * m_fSpeed * 20); // jump forward
	}
}

void MovementController::HandleMouseEvent(Event* pEvt)
{
	if (pEvt->m_ID == EventID::Mouse_Move_Event)
	{
		Mouse_Move_Event* pMouseEvt = (Mouse_Move_Event*) pEvt;
		//if (pMouseEvt->cursorPosChange[0] != 0 && pMouseEvt->cursorPosChange[1] != 0)
			//D3D11Renderer::GetInstance()->GetCamera()->rotateVPos(pMouseEvt->cursorPosChange[0] / 150.0f, pMouseEvt->cursorPosChange[1] / 150.0f);
	}
}

void MovementController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->Transform(trans);
}
