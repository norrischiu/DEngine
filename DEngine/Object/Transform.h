#ifndef TRANSFORM_H_
#define TRANSFORM_H_

// Engine include
#include "Object\Component.h"
#include "Graphics\Animation\Skeleton.h"
#include "Math\simdmath.h"

namespace DE
{

	class Transform : public Component
	{

	public:

		static const int ComponentID = ComponentID::TRANSFORM;

		Transform()
			: Component()
			, m_pParent(nullptr)
			, m_pJointParent(nullptr)
		{
			m_ID = ComponentID;

			//Handle hMatrix(sizeof(Matrix4));
			m_mWorldTransform = new Matrix4;
			m_mLocalTransform = new Matrix4;
		}

		// Inherited via Component
		void Update(float deltaTime) override
		{
			if (m_pParent)
			{
				Matrix4 scale;
				if (m_pJointParent)
				{
					scale.CreateScale(100.0f);
					*m_mWorldTransform = *m_mLocalTransform * *m_pParent * *m_pJointParent * scale;
				}
				else
				{
					*m_mWorldTransform = *m_mLocalTransform * *m_pParent;
				}
			}
		};

		void AttachTo(Transform* transform)
		{
			m_pParent = transform->m_mWorldTransform;
		};

		void AttachToJoint(Skeleton* skel, int jointIndex)
		{
			m_pJointParent = skel->m_vGlobalPose[jointIndex];
		}

	// Pointer to the parent attached

	//Transform*							m_pParent;

	Matrix4*								m_pParent;

	Matrix4*								m_pJointParent;

	// Local to world transform
	Matrix4*								m_mWorldTransform;

	// World to local transform
	Matrix4*								m_mLocalTransform;
};

}
#endif // !TRANSFORM_H_



