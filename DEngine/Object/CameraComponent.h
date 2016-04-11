// CameraComponent.h: the camera component that attach to a game object
#ifndef CAMERA_COMPONENT_H_
#define CAMERA_COMPONENT_H_

#include "Object/Component.h"
#include "GameObject/GameObject.h"
#include "Math/simdmath.h"
#include "Math/Frustum.h"
#include "Physics/cdSphere.h"
#include "Physics/cdAABB.h"

namespace DE
{

class CameraComponent : public Component
{

public:

	static const int ComponentID = ComponentID::CAMERA;

	// Overload constructor: perspective camera
	CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar);

	// Overload constructor: orthographic camera
	CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fRatio, const float fZNear, const float fZFar);

	void SetAsRendererCamera();

	// Inherited via Component
	void Update(float deltaTime) override;

	Matrix4 GetViewMatrix();
	Matrix4 GetPerspectiveMatrix();
	Matrix4 GetPVMatrix();
	Frustum GetFrustum()
	{
		return m_Frustum;
	}
	
	// World position
	inline Vector3 GetPosition()
	{
		if (m_pOwner)
		{
			Vector3 pos = m_vPos;
			pos.Transform(*m_pOwner->GetTransform());
			return pos;
		}
		return m_vPos;
	}

	// World lookat
	inline Vector3 GetWorldLookAt()
	{
		if (m_pOwner)
		{
			Vector3 lookat = m_vLookAt;
			lookat.Transform(*m_pOwner->GetTransform());
			return lookat;
		}
		return m_vLookAt;
	}

	inline Vector3 GetLocalPosition()
	{
		return m_vPos;
	}


	inline void SetLocalPosition(Vector3 pos)
	{
		m_vPos = pos;
	}

	inline Vector3 GetLocalLookAt()
	{
		return m_vLookAt;
	}

	inline void SetLookAt(Vector3 lookAt)
	{
		m_vLookAt = lookAt;
	}

	inline Vector3 GetUp()
	{
		return m_vUp;
	}

	~CameraComponent() {};

private:
	// Camera position in local space of game object owner
	Vector3						m_vPos;

	Vector3						m_vLookAt;
	Vector3						m_vUp;
	Matrix4						m_mPerspectiveProj;
	Frustum						m_Frustum;
};

};

#endif