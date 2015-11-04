// Camera.h: the class of the basic type of camera

#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/simdmath.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

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

class Camera
{
public:
	Camera(const Vector3& m_vPos, const Vector3& m_vLookAt, const Vector3& m_vUp);

	void rotateVLookAt(const CameraMove moveType, const float theta);
	void rotateVPos(const float thetaX, const float thetaY);
	void move(const CameraMove moveType, const float offset);
	
	Matrix4 GetViewMatrix();
	
	void pitch(const Vector3& vec);
	void row(const Vector3& vec);
	void yaw(const Vector3& vec);

	~Camera(){};

private:
	Vector3 m_vPos;
	Vector3 m_vLookAt;
	Vector3 m_vUp;
};

#endif