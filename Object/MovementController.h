#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

#include "Event\EventQueue.h"
#include "Math\SIMDMath.h"

class GameObject;

class MovementController
{

public:

	MovementController(GameObject* ptr)
	{
		m_pOwner = ptr;
		m_fSpeed = 1.0f;
	}

	void Update(float deltaTime);

private:

	void HandleKeyboardEvent(Event* pEvt);
	void HandleMouseEvent(Event* pEvt);

	void Move(Vector3 vTrans);

	GameObject*						m_pOwner;
	float							m_fSpeed;
	float							m_fDeltaTime;
};

#endif // !MOVEMENT_CONTROLLER_H_



