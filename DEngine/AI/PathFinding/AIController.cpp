#include "AIController.h"
#include "../../GameObject/GameObject.h"
#include "FlowFieldBuilder.h"

namespace DE
{

AIController::AIController(FlowField* flowField, Terrain* terrain)
	: Component(nullptr), m_flowField(flowField), m_terrain(terrain), m_enableAI(false)
{
	m_ID = ComponentID;
}


AIController::~AIController()
{
}

void AIController::Init()
{
	if (GetOwner())
	{
		LockPosition(GetOwner()->GetPosition());
		SetPositionOwner(GetOwner()->GetPosition(), GetOwner());
		m_enableAI = true;
	}
}

float AIController::angleBetween(Vector3 vec1, Vector3 vec2)
{
	if (vec1.iszero() && vec2.iszero()) {
		return 0.0f;
	}

	const float c = vec1.Dot(vec2) / vec1.Length() * vec2.Length();
	return acos(c > 1 ? 1 : c) * 180.0f / PI;
}

bool AIController::HasPositionChange(const Vector3& newPos, const Vector3& currPos)
{
	return !(newPos - currPos).iszero();
}

bool AIController::IsSlopeSteep(const Vector3& newPos, const Vector3& currPos)
{
	const Vector3 vec1 = (newPos - currPos).Normalize();
	const Vector3 vec2 = Vector3(vec1.GetX(), 0.0f, vec1.GetZ());
	const float angle = angleBetween(vec1, vec2);

	return false && angle > 30.0f;
}

bool AIController::IsPositionOwner(const Vector3& position)
{
	GameObject* owner = m_flowField->getPositionOwner(position);

	if (!owner) {
		return false;
	}

	return owner->GetGameObjectID() == GetOwner()->GetGameObjectID();
}

bool AIController::IsBlockedByOther(const Vector3& position)
{
	GameObject* owner = m_flowField->getPositionOwner(position);

	if (!owner) {
		return false;
	}

	return	owner->GetGameObjectID() != GetOwner()->GetGameObjectID();
}

bool AIController::IsDesintationArrived()
{
	const Vector3 currPos = GetOwner()->GetPosition();
	const Vector3 destination = m_flowField->getDestination();
	
	const Vector3 difference = Vector3(
		floor(abs(currPos.GetX() - destination.GetX())),
		floor(abs(currPos.GetY() - destination.GetY())),
		floor(abs(currPos.GetZ() - destination.GetZ()))
	);

	return difference.iszero();
}

bool AIController::IsActive()
{
	return m_enableAI;
}

void AIController::SetActive(const bool setActive)
{
	m_enableAI = setActive;
}

Vector3 AIController::LookUpDirection(const Vector3& currPos)
{
	return m_flowField->getDirection(currPos);
}

float AIController::LookUpHeight(const Vector3& currPos)
{
	if (m_terrain) {
		return m_terrain->GetHeight(currPos.GetX(), currPos.GetZ());
	}

	return 0.0f;
}

Vector3 AIController::GetNewPosition(const float deltaTime)
{
	Vector3 currPos = GetOwner()->GetPosition();
	Vector3 direction = LookUpDirection(currPos);
	Vector3 newPos = currPos + direction * deltaTime * 3.0f;

	const float newY = LookUpHeight(newPos);
	newPos.SetY(newY);

	return newPos;
}

void AIController::UpdateCamera()
{
	//Update Camera
	CameraComponent* camera = GetOwner()->GetComponent<DE::CameraComponent>();
	if (camera != nullptr)
	{
		const float owner_y = GetOwner()->GetPosition().GetY();
		const Vector3 camLocalPos = camera->GetLocalPosition();

		camera->SetLocalPosition(Vector3(
			camLocalPos.GetX(),
			LookUpHeight(camera->GetPosition()) - owner_y + 5.0f,
			camLocalPos.GetZ())
		);
	}
}

void AIController::SetPositionOwner(const Vector3& position, GameObject* gameObj)
{
	m_flowField->setPositionOwner(position, gameObj);
}

void AIController::UnLockPosition(const Vector3& position)
{
	m_flowField->setPositionMovable(position, true);
}

void AIController::LockPosition(const Vector3& position)
{
	m_flowField->setPositionMovable(position, false);
}

void AIController::Update(float deltaTime)
{
	if (!IsActive()) { 
		return; 
	}

	if (IsDesintationArrived()) { 
		return; 
	}

	const Vector3 currPos = GetOwner()->GetPosition();

	if (IsBlockedByOther(currPos)) {
		LockPosition(currPos);
		return;
	}

	UnLockPosition(currPos);

	const Vector3 newPos = GetNewPosition(deltaTime);

	if (!HasPositionChange(newPos, currPos)) {
		LockPosition(currPos);
		return; 
	}

	if (IsBlockedByOther(newPos)) { 
		LockPosition(currPos);
		return; 
	}

	if (IsSlopeSteep(newPos, currPos)) { 
		LockPosition(currPos);
		return; 
	}

	Move(newPos - currPos);
	UpdateCamera();
}

void AIController::Move(Vector3 vTrans)
{
	SetPositionOwner(GetOwner()->GetPosition(), nullptr);
	UnLockPosition(GetOwner()->GetPosition());

	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	GetOwner()->TransformBy(trans);

	SetPositionOwner(GetOwner()->GetPosition(), GetOwner());
	LockPosition(GetOwner()->GetPosition());
}

};
