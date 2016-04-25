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

	struct AIConfig
	{
		float maxCohesion;
		float minSeperation;
		float maxForce;
		float maxSpeed;
		Vector3 velocity;
		bool avoidanceDirection;
		bool enableAI;

		FlowField* flowField;
		Terrain* terrain;
	};

	AIController(AIConfig m_aiConfig);
	AIController(FlowField* flowField, Terrain* terrain = nullptr);
	~AIController();

	void Init();

	bool IsActive();
	void SetActive(const bool setActive);
	// Inherited via Component
	virtual void Update(float deltaTime) override;
	
	void UpdateCamera();

	Vector3 SteeringBehaviourAvoid();
	Vector3 SteeringBehaviourAlignment();
	Vector3 SteeringBehaviourSeek(const Vector3& centerOfMass);
	Vector3 SteeringBehaviourSeparation();
	Vector3 SteeringBehaviourCohesion();

	bool IsDesintationArrived();

	Vector3 LookUpDirection(const Vector3& currPos);
	float LookUpHeight(const Vector3& currPos);

	Vector3 GetNewPosition(const float deltaTime);
	Matrix4 DirVecToMatrix(const Vector3& direction);
	float AngleBetween(Vector3 vec1, Vector3 vec2);
	void Move(const Vector3& vTrans);

public:
	AIConfig m_aiConfig;
};

};