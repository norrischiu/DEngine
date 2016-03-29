// Camera.h: Wrapper as a game object by default has a single camera component
#ifndef CAMERA_H_
#define CAMERA_H_

#include "GameObject\GameObject.h"
#include "Object\CameraComponent.h"

namespace DE
{

class Camera : public GameObject
{
public:

	Camera(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar)
	{
		DE::Handle hCamera(sizeof(CameraComponent));
		new (hCamera) CameraComponent(vPos, vLookAt, vUp, fFov, fRatio, fZNear, fZFar);
		AddComponent((Component*)hCamera.Raw());
	}

	void SetAsRendererCamera()
	{
		GetComponent<CameraComponent>()->SetAsRendererCamera();
	}
};

};
#endif // !CAMERA_H_

