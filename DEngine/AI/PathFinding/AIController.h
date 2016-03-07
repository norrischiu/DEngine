#pragma once
#include "../../Object/Component.h"
#include "../../Graphics/VertexFormat.h"
#include "FlowField.h"

class AIController : public Component
{

public:
	static const int ComponentID = ComponentID::AI_CONTROLLER;

	AIController(FlowField& flowField);
	~AIController();


	// Inherited via Component
	virtual void Update(float deltaTime) override;
	void Move(Vector3 vTrans);

private:
	FlowField m_flowField;
};

