// Camera.h: the class of the basic type of camera

#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/simdmath.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

class Camera
{
public:
	Camera(Vector3 m_vPos, Vector3 m_vLookAt, Vector3 m_vUp);

	Matrix4 getViewMatrix();
	
	void update();
	void move(float offset);
	void pitch(Vector3 vec);
	void row(Vector3 vec);
	void yaw(Vector3 vec);

	~Camera(){};

private:
	Vector3 m_vPos;
	Vector3 m_vLookAt;
	Vector3 m_vUp;
	Quaternion m_qRotation;
};

#endif