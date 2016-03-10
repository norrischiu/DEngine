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

void AIController::Update(float deltaTime)
{
	Vector3 direction = m_flowField.getDirection(m_pOwner->GetPosition());
	Move(direction * deltaTime * 2.0f);
}

void AIController::Move(Vector3 vTrans)
{
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	m_pOwner->TransformBy(trans);
}

};