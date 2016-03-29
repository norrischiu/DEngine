#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

// Engine include
#include "Object\Component.h"
#include "Event\EventQueue.h"
#include "Math\simdmath.h"

namespace DE
{

class MovementController : public Component
{

public:

	static const int ComponentID = ComponentID::CONTROLLER;

	MovementController(float speed = 1.0f)
		: Component()
		, m_fSpeed(speed)
	{
		m_ID = ComponentID;
	}

	// Inherited via Component
	void Update(float deltaTime) override;

protected:

	virtual void HandleKeyboardEvent(Event* pEvt);
	virtual void HandleMouseEvent(Event* pEvt);

	virtual void Move(Vector3 vTrans);

	float							m_fSpeed;
	float							m_fDeltaTime;
};

};
#endif // !MOVEMENT_CONTROLLER_H_



