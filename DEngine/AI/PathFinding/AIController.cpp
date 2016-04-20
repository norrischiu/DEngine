#include "AIController.h"
#include "../../GameObject/GameObject.h"
#include "FlowFieldBuilder.h"

namespace DE
{

AIController::AIController(FlowField* flowField, Terrain* terrain)
	: Component(nullptr), m_flowField(flowField), m_terrain(terrain)
{
	m_ID = ComponentID;
}


AIController::~AIController()
{
}

void AIController::updateFlowField(const Vector3& start, const Vector3& destination)
{
	m_flowField = FlowFieldBuilder::getInstance()->generateFlowField(start, destination, m_flowField->getObstacles(), destination);
}

float AIController::LookUp(const float x, const float z)
{
	if (m_terrain) {
		return m_terrain->GetHeight(x, z);
	}

	return 0.0f;
}

float AIController::angleBetween(Vector3 vec1, Vector3 vec2)
{
	if (vec1.iszero() && vec2.iszero()) {
		return 0.0f;
	}

	const float c = vec1.Dot(vec2) / vec1.Length() * vec2.Length();
	return acos(c > 1 ? 1 : c) * 180.0f / PI;
}

void AIController::Update(float deltaTime)
{
	Vector3 currPos = m_pOwner->GetPosition();

	Vector3 direction = m_flowField->getDirection(currPos);
	Vector3 newPos = m_pOwner->GetPosition() + direction * deltaTime * 3.0f;
	const float newY = LookUp(newPos.GetX(), newPos.GetZ());
	newPos.SetY(newY);

	if (!(newPos - currPos).iszero())
	{
		const Vector3 vec1 = (newPos - currPos).Normalize();
		const Vector3 vec2 = Vector3(vec1.GetX(), 0.0f, vec1.GetZ());
		const float angle = angleBetween(vec1, vec2);

		if (true || angle < 30.0f)
		{
			Move(newPos - currPos);

			CameraComponent* cam = m_pOwner->GetComponent<DE::CameraComponent>();
			if (cam != nullptr)
			{
				const float cam_x = cam->GetPosition().GetX();
				const float cam_z = cam->GetPosition().GetZ();
				const float owner_y = m_pOwner->GetPosition().GetY();
				cam->SetLocalPosition(Vector3(0.0f, LookUp(cam_x, cam_z) - owner_y + 5.0f, 5.0f));
			}
		}
	}
}

void AIController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}

};
