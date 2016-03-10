#pragma once
#include "../../Object/Component.h"
#include "../../Graphics/VertexFormat.h"
#include "FlowField.h"

namespace DE
{

class AIController : public Component
{

public:
	static const int ComponentID = ComponentID::AI_CONTROLLER;

	AIController(FlowField& flowField);
	~AIController();


	// Inherited via Component
	virtual void Update(float deltaTime) override;
	float LookUp(const float x, const float z);
	float angleBetween(Vector3 vec1, Vector3 vec2);
	void Move(Vector3 vTrans);
	void updateFlowField(const Vector3& start, const Vector3& destination);

private:
	FlowField m_flowField;
};

};