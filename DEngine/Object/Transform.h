#ifndef TRANSFORM_H_
#define TRANSFORM_H_

// Engine include
#include "Object\Component.h"
#include "Math\simdmath.h"

namespace DE
{

class Transform : public Component
{

public:

	static const int ComponentID = ComponentID::TRANSFORM;

	Transform()
		:Component()
	{
		m_ID = ComponentID;

		m_mWorldTransform = new Matrix4;
		m_mLocalTransform = new Matrix4;
	}

	// Inherited via Component
	void Update(float deltaTime) override 
	{
		if (m_pParent)
		{
			*m_mWorldTransform = *(m_pParent->m_mWorldTransform) * *m_mLocalTransform;
		}
	};

	void AttachTo(Transform* transform)
	{
		m_pParent = transform;
	};

	// Pointer to the parent attached
	Transform*							m_pParent;

	// Local to world transform
	Matrix4*							m_mWorldTransform;

	// World to local transform
	Matrix4*							m_mLocalTransform;
};

}
#endif // !TRANSFORM_H_



