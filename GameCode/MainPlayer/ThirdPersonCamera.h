// ThirdPersonCamera.h: Inherit default camera to support third person follow camera
#ifndef THIRD_PERSON_CAMERA_H_
#define THIRD_PERSON_CAMERA_H_

#include "Object\Camera.h"
#include "GameObject\GameObject.h"

class ThirdPersonCamera : public DE::Camera
{

public:

	ThirdPersonCamera(const DE::Vector3& vPos, const DE::Vector3& vLookAt, const DE::Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar)
		: DE::Camera(vPos, vLookAt, vUp, fFov, fRatio, fZNear, fZFar)
		, m_vOffset(vPos)
		, m_fYaw(0.0f)
		, m_fPitch(0.0f)
	{
	}

	DE::Vector3							m_vOffset;

	float								m_fYaw;

	float								m_fPitch;

	float								m_fMaxPitch;

};


#endif // !THIRD_PERSON_CAMERA_H_

