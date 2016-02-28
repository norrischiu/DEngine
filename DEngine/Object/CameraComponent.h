// CameraComponent.h: the camera component that attach to a game object
#ifndef CAMERA_COMPONENT_H_
#define CAMERA_COMPONENT_H_

#include "Object/Component.h"
#include "GameObject/GameObject.h"
#include "Math/simdmath.h"
#include "Math/Frustum.h"
#include "Physics/cdSphere.h"
#include "Physics/cdAABB.h"

enum CameraMove {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	ROTATE_UP,
	ROTATE_DOWN,
	ROTATE_LEFT,
	ROTATE_RIGHT
};

enum CameraType {
	MOVE_CAMERA,
	DEGREE360_CAMERA,
	TOP_VIEW_CAMERA,
	BOTTOM_VIEW_CAMERA,
	LEFT_VIEW_CAMERA,
	RIGHT_VIEW_CAMERA,
	FRONT_VIEW_CAMERA,
	BACK_VIEW_CAMERA
};

class CameraComponent : public Component
{

public:

	static const int ComponentID = ComponentID::CAMERA;

	// Overload constructor: perspective camera
	CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar);

	// Overload constructor: orthographic camera
	CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fRatio, const float fZNear, const float fZFar);

	void SetAsRendererCamera();

	void rotateVLookAt(const CameraMove moveType, const float theta);
	void rotateVPos(const float thetaX, const float thetaY);
	void move(const CameraMove moveType, const float offset);
	
	// Inherited via Component
	void Update(float deltaTime) override;

	Matrix4 GetViewMatrix();
	Matrix4 GetPerspectiveMatrix();
	Matrix4 GetPVMatrix();
	Frustum GetFrustum()
	{
		return m_Frustum;
	}
	inline Vector3 GetPosition()
	{
		return m_vPos;
	}

	~CameraComponent(){};

private:
	// Camera position in local space of game object owner
	Vector3						m_vPos;

	Vector3						m_vLookAt;
	Vector3						m_vUp;
	Matrix4						m_mPerspectiveProj;
	Frustum						m_Frustum;
};

#endif