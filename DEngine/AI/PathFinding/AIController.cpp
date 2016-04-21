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

bool AIController::IsBlockedByOther(const Vector3& position)
{
	return	m_flowField->getPositionOwner(position) != nullptr &&
			m_flowField->getPositionOwner(position)->GetGameObjectID() != m_pOwner->GetGameObjectID();
}

bool AIController::IsDesintationArrived()
{
	const Vector3 currPos = m_pOwner->GetPosition();
	const Vector3 destination = m_flowField->getDestination();
	
	const Vector3 difference = Vector3(
		floor(abs(currPos.GetX() - destination.GetX())),
		floor(abs(currPos.GetY() - destination.GetY())),
		floor(abs(currPos.GetZ() - destination.GetZ()))
	);

	return difference.iszero();
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
	Vector3 direction = LookUpDirection(m_pOwner->GetPosition());
	Vector3 newPos = m_pOwner->GetPosition() + direction * deltaTime * 3.0f;
	const float newY = LookUpHeight(newPos);
	newPos.SetY(newY);

	return newPos;
}

void AIController::Update(float deltaTime)
{
	if (!m_enableAI) { return; }

	if (IsDesintationArrived()) { return; }

	Vector3 currPos = m_pOwner->GetPosition();
	Vector3 newPos = GetNewPosition(deltaTime);

	if (!HasPositionChange(newPos, currPos)) { return; }

	if (IsBlockedByOther(newPos)) { return; }

	/*
	while (!m_flowField->isPositionMovable(newPos))
	{
		newPos.SetX(newPos.GetX() * 2.0f / 3.0f);
		newPos.SetZ(newPos.GetZ() * 2.0f / 3.0f);
		newPos.SetY(LookUpHeight(newPos));
				
		if (std::abs(newPos.Length()) < std::numeric_limits<float>::epsilon()) {
			return;
		}
	}
	*/

	if (IsSlopeSteep(newPos, currPos)) { return; }

	m_flowField->setPositionMovable(m_pOwner->GetPosition(), true);
	Move(newPos - currPos);
	m_flowField->setPositionMovable(m_pOwner->GetPosition(), false);

	//Update Camera
	CameraComponent* camera = m_pOwner->GetComponent<DE::CameraComponent>();
	if (camera != nullptr)
	{
		const float owner_y = m_pOwner->GetPosition().GetY();
		const Vector3 camLocalPos = camera->GetLocalPosition();

		camera->SetLocalPosition(Vector3(
			camLocalPos.GetX(), 
			LookUpHeight(camera->GetPosition()) - owner_y + 5.0f,
			camLocalPos.GetZ())
		);
	}
}

void AIController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}

};
