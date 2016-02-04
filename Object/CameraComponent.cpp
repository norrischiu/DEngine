// CameraComponent.cpp

#include "CameraComponent.h"
#include "Graphics\D3D11Renderer.h"
#include "Game/GlobalInclude.h"
#include "GameObject\GameObject.h"
#include "Debug\DebugDrawing.h"
#include <sstream>
#include <string>

CameraComponent::CameraComponent(const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp, const float fFov, const float fRatio, const float fZNear, const float fZFar)
	: Component(nullptr)
	, m_vPos(vPos)
	, m_vLookAt(vLookAt)
	, m_vUp(vUp)
{
	m_mPerspectiveProj = Matrix4::PerspectiveProjection(fFov, fRatio, fZNear, fZFar);
	m_Frustum = Frustum(fFov, fRatio, fZNear, fZFar);
	m_ID = ComponentID;
}

void CameraComponent::SetAsRendererCamera()
{
	D3D11Renderer::GetInstance()->SetCamera(this);
}

Matrix4 CameraComponent::GetViewMatrix()
{
	Vector3 pos = m_vPos, lookat = m_vLookAt;
	pos.Transform(*m_pOwner->GetTransform());
	lookat.Transform(*m_pOwner->GetTransform());
	return Matrix4::LookAtMatrix(pos, lookat, m_vUp);
}

Matrix4 CameraComponent::GetPerspectiveMatrix()
{
	return m_mPerspectiveProj;
}

Matrix4 CameraComponent::GetPVMatrix()
{
	return m_mPerspectiveProj * GetViewMatrix();
}

void CameraComponent::Update(float deltaTime)
{
}

void CameraComponent::rotateVPos(const float thetaX, const float thetaY) {
	Matrix4 transform;
	m_vPos -= m_vLookAt;
	transform.CreateRotationX(thetaY);
	m_vPos.TransformAsVector(transform);
	transform.CreateRotationY(thetaX);
	m_vPos.TransformAsVector(transform);
	m_vPos += m_vLookAt;
}

void CameraComponent::rotateVLookAt(const CameraMove moveType, const float theta)
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

void CameraComponent::move(const CameraMove moveType, const float offset)
{
	float sign = (moveType == CameraMove::BACKWARD || moveType == CameraMove::RIGHT) ? -1.0f : 1.0f;
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
