// CameraComponent.cpp
#include "CameraComponent.h"

// Engine include
#include "Graphics\D3D11Renderer.h"
#include "GameObject\GameObject.h"
#include "GlobalInclude.h"

// C++ include
#include <sstream>
#include <string>

namespace DE
{

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

};