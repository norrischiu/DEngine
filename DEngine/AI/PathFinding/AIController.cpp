#include "AIController.h"
#include "../../GameObject/GameObject.h"
#include "FlowFieldBuilder.h"

namespace DE
{

AIController::AIController(FlowField& flowField)
	: Component(nullptr), m_flowField(flowField)
{
	m_ID = ComponentID;
}


AIController::~AIController()
{
}

void AIController::updateFlowField(const Vector3& start, const Vector3& destination)
{
	m_flowField = FlowFieldBuilder::getInstance()->generateFlowField(start, destination, m_flowField.getObstacles(), destination);
}

float AIController::LookUp(const float x, const float z)
{
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
	Vector3 direction = m_flowField.getDirection(m_pOwner->GetPosition());
	Move(direction * deltaTime * 2.0f);
}

void AIController::Move(Vector3 vTrans)
{
	Vector3 newPos = m_pOwner->GetPosition() + vTrans;
	const float newY = LookUp(newPos.GetX(), newPos.GetZ());
	newPos.SetY(newY);
	const Vector3 vec1 = (newPos - m_pOwner->GetPosition()).iszero() ? Vector3(0.0f, 0.0f, 0.0f) : (newPos - m_pOwner->GetPosition()).Normalize();
	const Vector3 vec2 = Vector3(vec1.GetX(), 0.0f, vec1.GetZ());
	const float angle = angleBetween(vec1, vec2);

	if (angle < 30.0f)
	{
		vTrans.SetY(newY - m_pOwner->GetPosition().GetY());
		Matrix4 trans;
		trans.CreateTranslation(vTrans);
		m_pOwner->TransformBy(trans);
	}
}

};