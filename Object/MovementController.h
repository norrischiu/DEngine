#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

#include "Object\Component.h"
#include "Event\EventQueue.h"
#include "Math\SIMDMath.h"

class MovementController : public Component
{

public:

	static const int ComponentID = ComponentID::CONTROLLER;

	MovementController()
		:Component()
	{
		m_fSpeed = 1.0f;
	}

	// Inherited via Component
	void Update(float deltaTime) override;

private:

	void HandleKeyboardEvent(Event* pEvt);
	void HandleMouseEvent(Event* pEvt);

	void Move(Vector3 vTrans);

	float							m_fSpeed;
	float							m_fDeltaTime;
};

#endif // !MOVEMENT_CONTROLLER_H_



