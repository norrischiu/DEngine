// CameraComponent.h: the class of the basic type of camera

#ifndef CAMERA_COMPONENT_H_
#define CAMERA_COMPONENT_H_

#include "Object/Component.h"
#include "Math/simdmath.h"
#include "Physics/cdSphere.h"

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

class Plane
{
public:

	Plane() {};
	// Points list in clockwise direction
	Plane(Vector3 a, Vector3 b, Vector3 c)
	{
		m_vNormal = Cross(b - a, c - b);
		m_vNormal.Normalize();
		m_fDist = a.Dot(m_vNormal);
	}

	bool IsInside(Sphere sphere)
	{
		float dist = m_vNormal.Dot(sphere.getCenter());
		return dist > -sphere.getRadius();
	}

private:

	Vector3						m_vNormal;
	float						m_fDist;
};

class Frustum
{
public:
	Frustum() {};
	Frustum(const float fFov, const float fRatio, const float fZNear, const float fZFar)
	{
		Reconstruct(fFov, fRatio, fZNear, fZFar);
	}

	// Bounding sphere culling
	bool Cull(Sphere sphere)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (!m_planes[i].IsInside(sphere))
			{
				return false;
			}
		}
		return true;
	}

private:

	void Reconstruct(const float fFov, const float fRatio, const float fZNear, const float fZFar)
	{
		Vector3 corners[8]; // clockwise from bottom left
		// top
		corners[0] = Vector3(-fZNear * tan(fFov), fZNear * tan(fFov), fZNear);
		corners[1] = Vector3(-fZFar * tan(fFov), fZFar * tan(fFov), fZFar);
		corners[2] = Vector3(fZFar * tan(fFov), fZFar * tan(fFov), fZFar);
		corners[3] = Vector3(fZNear * tan(fFov), fZNear * tan(fFov), fZNear);
		// bottom
		corners[0] = Vector3(-fZNear * tan(fFov), -fZNear * tan(fFov), fZNear);
		corners[1] = Vector3(-fZFar * tan(fFov), -fZFar * tan(fFov), fZFar);
		corners[2] = Vector3(fZFar * tan(fFov), -fZFar * tan(fFov), fZFar);
		corners[3] = Vector3(fZNear * tan(fFov), -fZNear * tan(fFov), fZNear);

		m_planes[0] = Plane(corners[3], corners[0], corners[4]);
		m_planes[1] = Plane(corners[1], corners[2], corners[6]);
		m_planes[2] = Plane(corners[0], corners[1], corners[5]);
		m_planes[3] = Plane(corners[2], corners[3], corners[7]);
		m_planes[4] = Plane(corners[2], corners[3], corners[0]);
		m_planes[5] = Plane(corners[4], corners[5], corners[6]);
	}

	Plane						m_planes[6];
};

class CameraComponent : public Component
{

public:

	static const int ComponentID = ComponentID::CAMERA;

	CameraComponent(const Vector3& m_vPos, const Vector3& m_vLookAt, const Vector3& m_vUp);

	// Overload constructor: perspective camera
	CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar);

	void SetAsRendererCamera();

	void rotateVLookAt(const CameraMove moveType, const float theta);
	void rotateVPos(const float thetaX, const float thetaY);
	void move(const CameraMove moveType, const float offset);
	
	// Inherited via Component
	void Update(float deltaTime) override {};

	Matrix4 GetViewMatrix();
	Matrix4 GetPerspectiveMatrix();
	Matrix4 GetPVMatrix();
	inline Vector3 GetPosition()
	{
		return m_vPos;
	}

	void pitch(const Vector3& vec);
	void row(const Vector3& vec);
	void yaw(const Vector3& vec);

	~CameraComponent(){};

private:
	Vector3						m_vPos;
	Vector3						m_vLookAt;
	Vector3						m_vUp;
	Matrix4						m_mPerspectiveProj;
	Frustum						m_Frustum;
};

#endif