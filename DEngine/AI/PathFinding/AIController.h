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

	void Init();

	bool IsActive();
	void SetActive(const bool setActive);
	// Inherited via Component
	virtual void Update(float deltaTime) override;
	
	void UpdateCamera();

	void LockCurrPosition();
	void LockPosition(const Vector3& position);
	void UnLockCurrPosition();
	void UnLockPosition(const Vector3& position);
	void SetPositionOwner(const Vector3& position, const int gameObjId);

	bool HasPositionChange(const Vector3& newPos, const Vector3& currPos);
	bool IsPositionOwner(const Vector3& currPos);
	bool IsNewPositionOwner(const Vector3& newPos);
	bool IsBlockedByOther(const Vector3& position);
	bool IsNewPositionBlockedByOther(const Vector3& newPos);
	bool IsDesintationArrived();
	bool IsSlopeSteep(const Vector3& newPos, const Vector3& currPos);

	Vector3 LookUpDirection(const Vector3& currPos);
	float LookUpHeight(const Vector3& currPos);

	Vector3 GetNewPosition(const float deltaTime);
	Matrix4 DirVecToMatrix(const Vector3& direction);
	float AngleBetween(Vector3 vec1, Vector3 vec2);
	void Move(Matrix4 rotation, Vector3 vTrans);

public:
	FlowField* m_flowField;
	Terrain* m_terrain;
	bool m_enableAI;
	float currRadian;
};

};