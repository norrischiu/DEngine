#pragma once
#include "../../Object/Component.h"
#include "../../Graphics/VertexFormat.h"
#include "../../Graphics/Terrain/Terrain.h"
#include "FlowField.h"

namespace DE
{

class AIController : public Component
{

public:
	static const int ComponentID = ComponentID::AI_CONTROLLER;

	AIController(FlowField* flowField, Terrain* terrain = nullptr);
	~AIController();

	void SetActive(const bool setActive);
	// Inherited via Component
	virtual void Update(float deltaTime) override;
	
	bool HasPositionChange(const Vector3& newPos, const Vector3& currPos);
	bool IsBlockedByOther(const Vector3& position);
	bool IsDesintationArrived();
	bool IsSlopeSteep(const Vector3& newPos, const Vector3& currPos);

	Vector3 LookUpDirection(const Vector3& currPos);
	float LookUpHeight(const Vector3& currPos);

	Vector3 GetNewPosition(const float deltaTime);

	float angleBetween(Vector3 vec1, Vector3 vec2);
	void Move(Vector3 vTrans);

public:
	FlowField* m_flowField;
	Terrain* m_terrain;
	bool m_enableAI;
};

};