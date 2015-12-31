// Camera.cpp

#include "Camera.h"
#include "Game/GlobalInclude.h"
#include <Windows.h> // get keyboard state
#include <sstream>
#include <string>

Camera::Camera(const Vector3& m_vPos, const Vector3& m_vLookAt, const Vector3& m_vUp)
	: m_vPos(m_vPos)
	, m_vLookAt(m_vLookAt)
	, m_vUp(m_vUp)
{
	m_mPerspectiveProj = Matrix4::PerspectiveProjection(PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);
}

Camera::Camera(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar)
	: m_vPos(vPos)
	, m_vLookAt(vLookAt)
	, m_vUp(vUp)
{
	m_mPerspectiveProj = Matrix4::PerspectiveProjection(fFov, fRatio, fZNear, fZFar);
	m_Frustum = Frustum(fFov, fRatio, fZNear, fZFar);
}

Matrix4 Camera::GetViewMatrix()
{
	return Matrix4::LookAtMatrix(m_vPos, m_vLookAt, m_vUp);
}

Matrix4 Camera::GetPerspectiveMatrix()
{
	return m_mPerspectiveProj;
}

Matrix4 Camera::GetPVMatrix()
{
	return m_mPerspectiveProj * GetViewMatrix();
}

void Camera::rotateVPos(const float thetaX, const float thetaY) {
	Matrix4 transform;
	m_vPos -= m_vLookAt;
	transform.CreateRotationX(thetaY);
	m_vPos.TransformAsVector(transform);
	transform.CreateRotationY(thetaX);
	m_vPos.TransformAsVector(transform);
	m_vPos += m_vLookAt;
}

void Camera::rotateVLookAt(const CameraMove moveType, const float theta)
	{
	float sign = (moveType == CameraMove::ROTATE_RIGHT || moveType == CameraMove::ROTATE_UP) ? 1.0f : -1.0f;
	m_vLookAt -= m_vPos;
	Matrix4 transform;

	switch (moveType) {
		case CameraMove::ROTATE_LEFT:
		case CameraMove::ROTATE_RIGHT:
			transform.CreateRotationY(sign * theta);
			break;
		case CameraMove::ROTATE_UP:
		case CameraMove::ROTATE_DOWN:
			transform.CreateRotationX(sign * theta);
			break;
	}

	m_vLookAt.TransformAsVector(transform);
	m_vLookAt += m_vPos;
}

void Camera::move(const CameraMove moveType, const float offset)
{
	float sign = (moveType == CameraMove::BACKWARD || moveType == CameraMove::RIGHT) ? 1.0f : -1.0f;
	Vector3 transform;

	switch (moveType) {
		case CameraMove::FORWARD:
		case CameraMove::BACKWARD:
			transform = (m_vLookAt - m_vPos).Normalize() * (sign * offset);
			break;
		case CameraMove::LEFT:
		case CameraMove::RIGHT:
			Matrix4 rotation;
			rotation.CreateRotationY(PI / 2.0f);
			transform = (m_vLookAt - m_vPos).Normalize();
			transform.TransformAsVector(rotation);
			transform = transform * (sign * offset);
			break;
	}
	
	m_vPos += transform;
	m_vLookAt += transform;
}