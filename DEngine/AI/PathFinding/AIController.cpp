#include "AIController.h"
#include "GameObject/GameWorld.h"
#include "GameObject/GameObject.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/HUD/HUD.h"
#include "FlowFieldBuilder.h"
#include "PositioningSystem.h"

namespace DE
{

AIController::AIController(FlowField* flowField, Terrain* terrain, PositioningSystem* positioningSystem)
	: Component(GetOwner())
{
	m_ID = ComponentID;

	m_aiConfig.minSeperation = 0.0f;
	m_aiConfig.maxCohesion = 0.0f;
	m_aiConfig.maxForce = 15.0f;
	m_aiConfig.maxSpeed = 5.0f;
	m_aiConfig.forceToApply = Vector3::Zero;
	m_aiConfig.velocity = Vector3::Zero;
	m_aiConfig.avoidanceDirection = NULL;
	m_aiConfig.flowField = flowField;
	m_aiConfig.terrain = terrain;
	m_aiConfig.positioningSystem = positioningSystem;
	m_aiConfig.enableAI = false;
	m_aiConfig.destination = flowField->getDestination();
}

AIController::AIController(const Vector3& destination, Terrain* terrain, PositioningSystem* positioningSystem)
	: Component(GetOwner())
{
	m_ID = ComponentID;

	m_aiConfig.minSeperation = 0.0f;
	m_aiConfig.maxCohesion = 0.0f;
	m_aiConfig.maxForce = 15.0f;
	m_aiConfig.maxSpeed = 5.0f;
	m_aiConfig.forceToApply = Vector3::Zero;
	m_aiConfig.velocity = Vector3::Zero;
	m_aiConfig.avoidanceDirection = NULL;
	m_aiConfig.flowField = nullptr;
	m_aiConfig.terrain = terrain;
	m_aiConfig.positioningSystem = positioningSystem;
	m_aiConfig.enableAI = false;
	m_aiConfig.destination = destination;
}


AIController::AIController(AIConfig aiConfig) 
	: m_aiConfig(aiConfig)
{
	m_ID = ComponentID;
}

AIController::~AIController()
{
}

void AIController::Init()
{
	if (GetOwner())
	{
		GameObject* gameObj = GetOwner();

		AABB aabb = gameObj->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
		Vector3 min = aabb.getMin();
		Vector3 max = aabb.getMax();
		Vector3 differentHalf = (min - max) * (1.0f / 2.0f);
		const float radius = abs((differentHalf.GetX() + differentHalf.GetZ()) / 2.0f);

		m_aiConfig.maxCohesion = 0.5f;
		m_aiConfig.minSeperation = radius * 4.0f + m_aiConfig.maxCohesion;

		if (m_aiConfig.positioningSystem)
		{
			m_aiConfig.positioningSystem->UpdatePositionOwner(gameObj->GetPosition(), gameObj->GetPosition(), gameObj->GetGameObjectID());
		}
	}

	m_aiConfig.enableAI = true;
}

float AIController::AngleBetween(Vector3 vec1, Vector3 vec2)
{
	if (vec1.iszero() || vec2.iszero()) {
		return 0.0f;
	}

	const float c = vec1.Dot(vec2) / vec1.Length() * vec2.Length();
	return acos(min(1, c));
}

Matrix4 AIController::GetRotationMatrix(const Vector3& direction)
{
	Matrix4 rotationMatrix = Matrix4::Identity;

	Vector3 cross = Cross(
		GetOwner()->GetTransform()->GetForward().Normal(), 
		direction
	);

	const float dot = cross.Dot(Vector3(0.0f, 1.0f, 0.0f));
	float theta = asinf(cross.Length());
	
	if (dot < 0.0f)
	{
		theta = 2 * PI - theta;
	}

	rotationMatrix = Quaternion(Vector3(0.0f, 1.0f, 0.0f), theta).GetRotationMatrix();

	return rotationMatrix;
}


bool AIController::IsDesintationArrived()
{
	const Vector3 currPos = GetOwner()->GetPosition();
	const Vector3 destination = m_aiConfig.destination;
	
	const Vector3 difference = Vector3(
		floor(abs(currPos.GetX() - destination.GetX())),
		0.0f,
		floor(abs(currPos.GetZ() - destination.GetZ()))
	);

	return difference.iszero();
}

bool AIController::IsActive()
{
	return m_aiConfig.enableAI;
}

void AIController::SetActive(const bool setActive)
{
	m_aiConfig.enableAI = setActive;
}

Vector3 AIController::LookUpDirection(const Vector3& position)
{
	return m_aiConfig.flowField->getDirection(m_aiConfig.velocity.Normal());
}

float AIController::LookUpHeight(const Vector3& position)
{
	if (m_aiConfig.terrain) {
		return m_aiConfig.terrain->GetHeight(position.GetX(), position.GetZ());
	}

	return 0.0f;
}

bool AIController::IsBoundingBoxCollide(GameObject* gameObj1, GameObject* gameObj2)
{
	Collide collide;

	Matrix4 trans;
	Vector3 forward = Vector3::UnitZ;
	forward = forward * m_aiConfig.velocity.Length();
	trans.CreateTranslation(forward);

	AABB gameObj1_aabb = *((AABB*)gameObj1->GetComponent<Body>());
	gameObj1_aabb.Transform(*gameObj1->GetTransform() * trans);
	Vector3 gameObj1_min = gameObj1_aabb.getMin();
	gameObj1_min.SetY(1.0f);
	gameObj1_aabb.setMin(gameObj1_min);
	Vector3 gameObj1_max = gameObj1_aabb.getMax();
	gameObj1_max.SetY(2.0f);
	gameObj1_aabb.setMax(gameObj1_max);

	AABB gameObj2_aabb = *((AABB*)gameObj2->GetComponent<Body>());
	gameObj2_aabb.Transform(*gameObj2->GetTransform());
	Vector3 gameObj2_min = gameObj2_aabb.getMin();
	gameObj2_min.SetY(1.0f);
	gameObj2_aabb.setMin(gameObj2_min);
	Vector3 gameObj2_max = gameObj2_aabb.getMax();
	gameObj2_max.SetY(2.0f);
	gameObj2_aabb.setMax(gameObj2_max);

	collide.boxBoxCollide(&gameObj1_aabb, &gameObj2_aabb);

	const bool result = collide.getCollide();

	return result;
}

bool AIController::IsCollideWithAnyGameObject(GameObject* gameObj)
{
	for (auto itr : GetPossibleCollisionGameObject())
	{
		if (itr.first != gameObj && itr.first->GetComponent<Body>())
		{
			if (IsBoundingBoxCollide(gameObj, itr.first))
			{
				return true;
			}
		}
	}
	
	return false;
}

std::unordered_map<GameObject*, bool> AIController::GetPossibleCollisionGameObject()
{
	const int gameObjId = GetOwner()->GetGameObjectID();

	std::unordered_map<GameObject*, bool> possibleList;

	Matrix4 trans;
	Vector3 offset = m_aiConfig.velocity;
	trans.CreateTranslation(offset);

	AABB boundingBox = *GetOwner()->GetComponent<AABB>();
	boundingBox.Transform(*GetOwner()->GetTransform() /* * trans */);
	const Vector3 minXYZ = boundingBox.getMin();
	const Vector3 maxXYZ = boundingBox.getMax();

	const int minX = (int) floor(minXYZ.GetX());
	const int maxX = (int) ceil(maxXYZ.GetX());
	const int minZ = (int) floor(minXYZ.GetZ());
	const int maxZ = (int) ceil(maxXYZ.GetZ());

	for (int x = minX; x < maxX; x++)
	{
		for (int z = minZ; z < maxZ; z++)
		{
			for (int ownerId : m_aiConfig.positioningSystem->GetPositionOwnerId(Vector3(x, 0.0f, z)))
			{
				if (ownerId != gameObjId)
				{
					GameObject* ownerGameObj = GameWorld::GetInstance()->GetGameObjectAt(ownerId);
					if (ownerGameObj != nullptr)
					{
						possibleList[ownerGameObj] = true;
					}
				}
			}
		}
	}

	return possibleList;
}

Vector3 AIController::SteerTowards(const Vector3& direction)
{
	Vector3 desiredDirection = direction;

	//Multiply our direction by speed for our desired speed
	Vector3 desiredVelocity = desiredDirection * m_aiConfig.maxSpeed;

	//The velocity change we want
	Vector3 velocityChange = desiredVelocity - m_aiConfig.velocity;

	//Convert to a force
	return velocityChange * (m_aiConfig.maxForce / m_aiConfig.maxSpeed);
}

Vector3 AIController::SteeringBehaviourFlowField()
{
	//bilinear-interpolation
	Vector3 pos = GetOwner()->GetPosition();

	Vector3 f00 = m_aiConfig.flowField->isPositionMovable(pos) ? m_aiConfig.flowField->getDirection(pos) : Vector3::Zero;
	Vector3 f01 = m_aiConfig.flowField->isPositionMovable(pos + Vector3(0.0f, 0.0f, 1.0f)) ? m_aiConfig.flowField->getDirection(pos + Vector3(0.0f, 0.0f, 1.0f)) : Vector3::Zero;
	Vector3 f10 = m_aiConfig.flowField->isPositionMovable(pos + Vector3(1.0f, 0.0f, 0.0f)) ? m_aiConfig.flowField->getDirection(pos + Vector3(1.0f, 0.0f, 0.0f)) : Vector3::Zero;
	Vector3 f11 = m_aiConfig.flowField->isPositionMovable(pos + Vector3(1.0f, 0.0f, 1.0f)) ? m_aiConfig.flowField->getDirection(pos + Vector3(1.0f, 0.0f, 1.0f)) : Vector3::Zero;

	//Do the x interpolations
	const float xWeight = pos.GetX() - floor(pos.GetX());

	Vector3 top = f00 * (1.0f - xWeight) + (f10 * xWeight);
	Vector3 bottom = f01 * (1.0f - xWeight) + (f11 * xWeight);

	//Do the y interpolation
	const float yWeight = pos.GetZ() - floor(pos.GetZ());

	//This is now the direction we want to be travelling in (needs to be normalized)
	Vector3 desiredDirection = top * (1.0f - yWeight) + (bottom * yWeight);
	desiredDirection.Normalize();

	//If we are centered on a grid square with no vector this will happen
	if (std::isnan(desiredDirection.LengthSquared())) {
		return Vector3::Zero;
	}

	return SteerTowards(desiredDirection);
}

Vector3 AIController::SteeringBehaviourAvoid()
{
	GameObject* gameObj = GetOwner();

	AABB gameObjBoundingBox = gameObj->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
	Vector3 gameObjRadius = (gameObjBoundingBox.getMax() - gameObjBoundingBox.getMin()) * (1.0f / 2.0f);

	/*
	//If we aren't moving much, we don't need to try avoid
	if (m_aiConfig.velocity.LengthSquared() <= ((gameObjRadius.GetX() + gameObjRadius.GetZ()) / 2.0f)) {
		return Vector3::Zero;
	}
	*/

	//Do some ray casts to work out what is in front of us
	float minFraction = (std::numeric_limits<float>::max)();
	GameObject* closestFixture = nullptr;

	if (!m_aiConfig.positioningSystem)
	{
		for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
		{
			if (itr != gameObj && itr->GetComponent<Body>())
			{
				if (IsBoundingBoxCollide(gameObj, itr))
				{
					((TextBox*)HUD::getInstance()->getHUDElementById("debug2"))->setText("%d & %d Collided", itr->GetGameObjectID(), gameObj->GetGameObjectID());

					Vector3 difference = (gameObj->GetPosition() - itr->GetPosition());
					difference.SetY(0.0f);
					const float fraction = difference.Length();

					if (fraction < minFraction)
					{
						minFraction = fraction;
						closestFixture = itr;
					}
				}
			}
		}
	}
	else
	{
		for (auto itr : GetPossibleCollisionGameObject())
		{
			if (itr.first != gameObj && itr.first->GetComponent<Body>())
			{
				if (IsBoundingBoxCollide(gameObj, itr.first))
				{
					((TextBox*)HUD::getInstance()->getHUDElementById("debug2"))->setText("%d & %d Collided", itr.first->GetGameObjectID(), gameObj->GetGameObjectID());

					Vector3 difference = (gameObj->GetPosition() - itr.first->GetPosition());
					difference.SetY(0.0f);
					const float fraction = difference.Length();

					if (fraction < minFraction)
					{
						minFraction = fraction;
						closestFixture = itr.first;
					}
				}
			}
		}
	}

	//If we aren't going to collide, we don't need to avoid
	if (closestFixture == nullptr) {
		return Vector3::Zero;
	}

	Vector3 resultVector;
	Body collisionBody = *closestFixture->GetComponent<Body>();
	float ourVelocityLengthSquared = m_aiConfig.velocity.LengthSquared();

	//Add our velocity and the other Agents velocity
	//If this makes the total length longer than the individual length of one of them, then we are going in the same direction
	Vector3 combinedVelocity = m_aiConfig.velocity + collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.velocity;
	float combinedVelocityLengthSquared = combinedVelocity.LengthSquared();

	//We are going in the same direction and they aren't avoiding
	if (combinedVelocityLengthSquared > ourVelocityLengthSquared && closestFixture->GetComponent<AIController>()->m_aiConfig.avoidanceDirection == NULL) {
		return Vector3::Zero;
	}

	//We need to Steer to go around it, we assume the other shape is also a circle

	Vector3 vectorInOtherDirection = collisionBody.GetOwner()->GetPosition() - gameObj->GetPosition();
	vectorInOtherDirection.SetY(0.0f);

	//Are we more left or right of them
	bool isLeft;
	if (collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.avoidanceDirection != NULL) {
		//If they are avoiding, avoid with the same direction as them, so we go the opposite way
		isLeft = collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.avoidanceDirection;
	}
	else {
		const float dot = m_aiConfig.velocity.GetX() * -vectorInOtherDirection.GetZ() + m_aiConfig.velocity.GetZ() * vectorInOtherDirection.GetX();
		isLeft = dot > 0.0f;
	}
	m_aiConfig.avoidanceDirection = isLeft;

	//Calculate a right angle of the vector between us
	resultVector = isLeft ? Vector3(-vectorInOtherDirection.GetZ(), 0.0f, vectorInOtherDirection.GetX()) : Vector3(vectorInOtherDirection.GetZ(), 0.0f, -vectorInOtherDirection.GetX());
	resultVector.Normalize();

	//Move it out based on our radius + theirs
	AABB closestFixtureBoundingBox = closestFixture->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
	Vector3 closestFixtureRadius = (closestFixtureBoundingBox.getMax() - closestFixtureBoundingBox.getMin()) * (1.0f / 2.0f);

	resultVector = resultVector * ((gameObjRadius.GetX() + gameObjRadius.GetZ()) / 2.0f + (closestFixtureRadius.GetX() + closestFixtureRadius.GetZ()) / 2.0f);

	//Steer torwards it, increasing force based on how close we are
	const Vector3 resultForce = SteerTowards(resultVector) * (1.0f / minFraction);
	return resultForce;
}

Vector3 AIController::SteeringBehaviourAlignment()
{
	GameObject* gameObj = GetOwner();

	Vector3 averageHeading = Vector3::Zero;
	int neighboursCount = 0;

	//for each of our neighbours (including ourself)
	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		Vector3 difference = (gameObj->GetPosition() - itr->GetPosition());
		difference.SetY(0.0f);
		float distance = difference.Length();

		//That are within the max distance and are moving
		AIController* itrAIController = itr->GetComponent<AIController>();
		if (distance < m_aiConfig.maxCohesion && itrAIController)
		{
			//if (gameObj->GetComponent<AIController>()->m_aiConfig.flowField == itrAIController->m_aiConfig.flowField)
			//{
				Vector3 itrVelocity = itrAIController->m_aiConfig.velocity;
				if (!itrVelocity.iszero()) {
					//Sum up our headings
					averageHeading = averageHeading + itrVelocity.Normal();
					neighboursCount++;
				}
			//}
		}
	}

	if (neighboursCount == 0) {
		return Vector3::Zero;
	}

	//Divide to get the average heading
	averageHeading = averageHeading * (1.0f / neighboursCount);

	//Steer towards that heading
	const Vector3 resultForce = SteerTowards(averageHeading);
	return resultForce;
}

Vector3 AIController::SteeringBehaviourSeek(const Vector3& centerOfMass)
{
	GameObject* gameObj = GetOwner();

	//Desired change of location
	Vector3 desired = centerOfMass - gameObj->GetPosition();
	desired.SetY(0.0f);

	//Desired velocity (move there at maximum speed)
	if (!desired.iszero())
	{
		desired = desired * (m_aiConfig.maxSpeed / desired.Length());
	}

	//The velocity change we want
	Vector3 force = desired - m_aiConfig.velocity;

	//Convert to a force
	const Vector3 resultForce = force * (m_aiConfig.maxForce / m_aiConfig.maxSpeed);
	return resultForce;
}

Vector3 AIController::SteeringBehaviourCohesion()
{
	GameObject* gameObj = GetOwner();

	//Start with just our position
	Vector3 centerOfMass = gameObj->GetPosition();
	int neighboursCount = 1;

	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		if (itr != gameObj)
		{
			Vector3 difference = (gameObj->GetPosition() - itr->GetPosition());
			difference.SetY(0.0f);
			float distance = difference.Length();

			if (distance < m_aiConfig.maxCohesion) {
				//sum up the position of our neighbours
				centerOfMass = centerOfMass + itr->GetPosition();
				neighboursCount++;
			}
		}
	}

	if (neighboursCount == 1) {
		return Vector3::Zero;
	}

	//Get the average position of ourself and our neighbours
	centerOfMass = centerOfMass * (1.0f / neighboursCount);
	centerOfMass.SetY(0.0f);

	//seek that position
	const Vector3 resultForce = SteeringBehaviourSeek(centerOfMass);
	return resultForce;
}

Vector3 AIController::SteeringBehaviourSeparation()
{
	GameObject* gameObj = GetOwner();

	Vector3 totalForce = Vector3::Zero;
	int neighboursCount = 0;

	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		if (itr != gameObj)
		{
			Vector3 difference = (gameObj->GetPosition() - itr->GetPosition());
			difference.SetY(0.0f);
			float distance = difference.Length();

			if (distance < m_aiConfig.minSeperation && distance > 0.0f)
			{
				AABB boundingBox = gameObj->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
				Vector3 radius = (boundingBox.getMax() - boundingBox.getMin()) * (1.0f / 2.0f);

				Vector3 pushForce = gameObj->GetPosition() - itr->GetPosition();
				pushForce.SetY(0.0f);
				totalForce = totalForce + (pushForce * ((radius.GetX() + radius.GetZ() / 2.0f)));
				neighboursCount++;
			}
		}
	}

	if (neighboursCount == 0) {
		return Vector3::Zero;
	}

	//Normalise the force back down and then back up based on the maximum force
	totalForce = totalForce * (1.0f / neighboursCount);

	const Vector3 resultForce = totalForce * m_aiConfig.maxForce;
	return resultForce;
}


void AIController::UpdateCamera()
{
	//Update Camera
	CameraComponent* camera = GetOwner()->GetComponent<DE::CameraComponent>();

	if (camera != nullptr)
	{
		const float owner_y = GetOwner()->GetPosition().GetY();
		const Vector3 camLocalPos = camera->GetLocalPosition();
		static float initLocalY = camLocalPos.GetY();

		camera->SetLocalPosition(Vector3(
			camLocalPos.GetX(),
			LookUpHeight(camera->GetPosition()) - owner_y + initLocalY,
			camLocalPos.GetZ()
		));
	}
}

void AIController::Update(float deltaTime)
{
	if (!m_aiConfig.enableAI) { return; };

	if (IsDesintationArrived()) { return; }

	//Work out our behaviours
	if (m_aiConfig.flowField)
	{
		Vector3 flowFieldForce = SteeringBehaviourFlowField() * 1.0f;
		m_aiConfig.forceToApply = flowFieldForce;
	}
	else
	{
		Vector3 seekForce = SteeringBehaviourSeek(m_aiConfig.destination) * 1.2f;
		m_aiConfig.forceToApply = seekForce;
	}

	// Move away from those entities we are too close to.
	Vector3 separationForce = SteeringBehaviourSeparation() * 2.0f;

	//Move nearer to those entities we are near but not near enough to
	Vector3 cohesionForce = SteeringBehaviourCohesion() * 0.2f;

	//Change our direction to be closer to our neighbours;
	Vector3 alignmentForce = SteeringBehaviourAlignment() * 0.5f;

	//Avoid collision
	Vector3 avoidanceForce = SteeringBehaviourAvoid() * 5.0f;

	//Combine them to come up with a total force to apply, decreasing the effect of cohesion
	m_aiConfig.forceToApply += separationForce + cohesionForce + alignmentForce + avoidanceForce;

	((TextBox*)HUD::getInstance()->getHUDElementById("debug1"))->setText(
		"Seperation x: %.3f, z: %.3f\nCohesion x: %.3f, z: %.3f\nAlignment x: %.3f, z: %.3f\nAvoid x: %.3f, z:%.3f\nforceToApply: x: %.3f, z: %.3f", 
		separationForce.GetX(), separationForce.GetZ(),
		cohesionForce.GetX(), cohesionForce.GetZ(),
		alignmentForce.GetX(), alignmentForce.GetZ(),
		avoidanceForce.GetX(), avoidanceForce.GetZ(),
		m_aiConfig.forceToApply.GetX(), m_aiConfig.forceToApply.GetZ()
	);
}

void AIController::Move(const float deltaTime)
{
	if (IsDesintationArrived()) { return; }

	m_aiConfig.velocity = m_aiConfig.velocity + m_aiConfig.forceToApply * deltaTime;

	const float speed = m_aiConfig.velocity.Length();
	if (speed > m_aiConfig.maxSpeed)
	{
		m_aiConfig.velocity = m_aiConfig.velocity * (m_aiConfig.maxSpeed / speed);
	}

	m_aiConfig.velocity = m_aiConfig.velocity * deltaTime;

	const Vector3 currPos = GetOwner()->GetPosition();

	//rotation
	GetOwner()->TransformBy(GetRotationMatrix(m_aiConfig.velocity.Normal()));

	//translation
	Matrix4 trans;
	Vector3 forward = Vector3::UnitZ;
	forward = forward * m_aiConfig.velocity.Length();
	trans.CreateTranslation(forward);
	GetOwner()->TransformBy(trans);

	//look up height
	Vector3 newPos = GetOwner()->GetPosition();
	newPos.SetY(LookUpHeight(newPos));
	GetOwner()->SetPosition(newPos);

	if (m_aiConfig.positioningSystem)
	{
		m_aiConfig.positioningSystem->UpdatePositionOwner(currPos, newPos, GetOwner()->GetGameObjectID());
	}

	UpdateCamera();

	m_aiConfig.avoidanceDirection = NULL;
}

};
