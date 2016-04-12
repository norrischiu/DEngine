#ifndef AIBEHAVIOR_H_
#define AIBEHAVIOR_H_

// Engine include
#include "DEngine\Object\Component.h"
#include "DEngine\Math\simdmath.h"

class AIBehavior : public DE::Component
{

public:

	AIBehavior()
		: DE::Component()
	{
		m_ID = DE::ComponentID::CUSTOM_COMPONENT_1;
		time = 0.0f;
	};

private:

	void BossBehavior();

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	void Move(DE::Vector3 move);

	float time;
};

#endif // !AIBEHAVIOR_H_
