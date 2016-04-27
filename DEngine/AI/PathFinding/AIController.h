#pragma once
#include "Object/Component.h"
#include "Graphics/VertexFormat.h"
#include "Graphics/Terrain/Terrain.h"
#include "PositioningSystem.h"
#include "FlowField.h"
#include <unordered_map>

namespace DE
{

class AIController : public Component
{

public:
	static const int ComponentID = ComponentID::AI_CONTROLLER;

	struct AIConfig
	{
		//Move away from those entities we are too close too
		float minSeperation;
		//Move nearer to those entities we are near but not near enough to
		float maxCohesion;
		//rate of acceleration
		float maxForce;
		//grid squares / second
		float maxSpeed;
		//force to apply;
		Vector3 forceToApply;
		//velocity
		Vector3 velocity;
		//avoidance direction
		bool avoidanceDirection;
		//flowfield for lookup direction
		FlowField* flowField;
		//terrain for lookup height
		Terrain* terrain;
		//positioning system
		PositioningSystem* positioningSystem;
		//destination;
		Vector3 destination;
		//enable flag;
		bool enableAI;
	};

	AIController(AIConfig m_aiConfig);
	AIController(const Vector3& destination, Terrain* terrain, PositioningSystem* positioningSystem = nullptr);
	AIController(FlowField* flowField, Terrain* terrain, PositioningSystem* positioningSystem = nullptr);
	~AIController();

	void Init();

	bool IsActive();
	void SetActive(const bool setActive);
	// Inherited via Component
	virtual void Update(float deltaTime) override;
	
	void UpdateCamera();

	bool IsBoundingBoxCollide(GameObject* gameObj1, GameObject* gameObj2);
	bool IsCollideWithAnyGameObject(GameObject* gameObj);
	
	std::unordered_map<GameObject*, bool> GetPossibleCollisionGameObject();

	//Avoid collision
	Vector3 SteerTowards(const Vector3& desiredDirection);
	Vector3 SteeringBehaviourFlowField();
	Vector3 SteeringBehaviourAvoid();
	Vector3 SteeringBehaviourAlignment();
	//Move towards a fixed point
	Vector3 SteeringBehaviourSeek(const Vector3& centerOfMass);
	Vector3 SteeringBehaviourSeparation();
	Vector3 SteeringBehaviourCohesion();

	bool IsDesintationArrived();

	Vector3 LookUpDirection(const Vector3& position);
	float LookUpHeight(const Vector3& position);

	Matrix4 GetRotationMatrix(const Vector3& direction);
	float AngleBetween(Vector3 vec1, Vector3 vec2);
	void Move(const float deltaTime);

public:
	AIConfig m_aiConfig;
};

};