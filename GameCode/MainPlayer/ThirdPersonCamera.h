// ThirdPersonCamera.h: Inherit default camera to support third person follow camera
#ifndef THIRD_PERSON_CAMERA_H_
#define THIRD_PERSON_CAMERA_H_

// Engine include
#include "DEngine\Object\Camera.h"
#include "DEngine\GameObject\GameObject.h"
#include "DEngine\GameObject\GameWorld.h"

// Debug include
#include "DEngine\DebugRenderer\DEBUG_RENDERER.h"

class ThirdPersonCamera : public DE::Camera
{

public:

	ThirdPersonCamera(const DE::Vector3& vPos, const DE::Vector3& vLookAt, const DE::Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar)
		: DE::Camera(vPos, vLookAt, vUp, fFov, fRatio, fZNear, fZFar)
		, m_vOffset(DE::Vector3(0.0f, 3.0f, -3.0f))
		, m_fYaw(0.0f)
		, m_fPitch(0.0f)
	{
	}

	void Update(float deltaTime) override
	{
		DE::Camera::Update(deltaTime);

		DE::Matrix4 trans;
		DE::Vector3 cameraDir = (GetComponent<DE::CameraComponent>()->GetWorldLookAt() - GetPosition()).Normalize();
		DE::Vector3 rayEnd = GetPosition() + cameraDir * 0.1f;
		DE::Vector3 rayStart = GetPosition();
		while (DE::GameWorld::GetInstance()->RayCast(rayStart, rayEnd))
		{
			trans.CreateTranslation(cameraDir);
			rayStart.Transform(trans);
			rayEnd = rayStart + cameraDir * 0.1f;
		}
		SetPosition(rayStart);
	}

	DE::Vector3							m_vOffset;

	float								m_fYaw;

	float								m_fPitch;

	float								m_fMaxPitch;

};


#endif // !THIRD_PERSON_CAMERA_H_

