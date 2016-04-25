#include "AIController.h"
#include "../../GameObject/GameWorld.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/MeshComponent.h"
#include "FlowFieldBuilder.h"
#include "FlowFieldManager.h"

namespace DE
{

AIController::AIController(FlowField* flowField, Terrain* terrain)
	: Component(GetOwner())
{
	m_ID = ComponentID;

	m_aiConfig.maxCohesion = 5.0f;
	m_aiConfig.maxForce = 30.0f;
	m_aiConfig.maxSpeed = 10.0f;
	m_aiConfig.minSeperation = 5.0f;
	m_aiConfig.velocity = Vector3(0.0f, 0.0f, 0.0f);
	m_aiConfig.flowField = flowField;
	m_aiConfig.terrain = terrain;
}

AIController::AIController(AIConfig aiConfig) : m_aiConfig(aiConfig)
{

}

AIController::~AIController()
{
}

void AIController::Init()
{
	if (GetOwner())
	{
		m_aiConfig.enableAI = true;
	}
}

float AIController::AngleBetween(Vector3 vec1, Vector3 vec2)
{
	if (vec1.iszero() && vec2.iszero()) {
		return 0.0f;
	}

	const float c = vec1.Dot(vec2) / vec1.Length() * vec2.Length();
	return acos(min(1, c));
}

Matrix4 AIController::DirVecToMatrix(const Vector3& direction)
{
	const Vector3 vec1 = direction;
	const Vector3 vec2 = Vector3(0.0f, 0.0f, vec1.GetZ());
	const float angle = AngleBetween(vec1, vec2);

	Matrix4 rotationMatrix = Quaternion(D3D11Renderer::GetInstance()->GetCamera()->GetUp(), angle).GetRotationMatrix();

	return rotationMatrix;
}


bool AIController::IsDesintationArrived()
{
	const Vector3 currPos = GetOwner()->GetPosition();
	const Vector3 destination = m_aiConfig.flowField->getDestination();
	
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

Vector3 AIController::LookUpDirection(const Vector3& currPos)
{
	return m_aiConfig.flowField->getDirection(currPos);
}

float AIController::LookUpHeight(const Vector3& currPos)
{
	if (m_aiConfig.terrain) {
		return m_aiConfig.terrain->GetHeight(currPos.GetX(), currPos.GetZ());
	}

	return 0.0f;
}

Vector3 AIController::SteeringBehaviourAvoid()
{
	GameObject* gameObj = GetOwner();

	AABB gameObjBoundingBox = gameObj->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
	Vector3 gameObjRadius = (gameObjBoundingBox.getMax() - gameObjBoundingBox.getMin()) * (1.0f / 2.0f);

	if (m_aiConfig.velocity.LengthSquared() <= ((gameObjRadius.GetX() + gameObjRadius.GetZ()) / 2.0f)) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//Do some ray casts to work out what is in front of us
	float minFraction = 3.0f;
	GameObject* closestFixture = nullptr;

	int minDistance = (std::numeric_limits<int>::max)();
	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		if (itr != gameObj)
		{
			gameObj->collision(itr);
			const float fraction = gameObj->getContact()->getDistance();
			if (fraction < minFraction)
			{
				minFraction = fraction;
				closestFixture = itr;
			}
		}
	}

	//If we aren't going to collide, we don't need to avoid
	if (closestFixture == nullptr) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 resultVector;
	Body collisionBody = *closestFixture->GetComponent<Body>();
	float ourVelocityLengthSquared = m_aiConfig.velocity.LengthSquared();

	//Add our velocity and the other Agents velocity
	//If this makes the total length longer than the individual length of one of them, then we are going in the same direction
	Vector3 combinedVelocity = m_aiConfig.velocity + collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.velocity;
	float combinedVelocityLengthSquared = combinedVelocity.LengthSquared();

	//We are going in the same direction and they aren't avoiding
	if (combinedVelocityLengthSquared > ourVelocityLengthSquared && closestFixture->GetComponent<AIController>()->m_aiConfig.avoidanceDirection != NULL) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//We need to Steer to go around it, we assume the other shape is also a circle

	Vector3 vectorInOtherDirection = collisionBody.GetOwner()->GetPosition() - gameObj->GetPosition();

	//Are we more left or right of them
	bool isLeft;
	if (!collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.avoidanceDirection != NULL) {
		//If they are avoiding, avoid with the same direction as them, so we go the opposite way
		isLeft = collisionBody.GetOwner()->GetComponent<AIController>()->m_aiConfig.avoidanceDirection;
	}
	else {
		const int dot = gameObj->GetComponent<AIController>()->m_aiConfig.velocity.GetX() * -vectorInOtherDirection.GetZ() + gameObj->GetComponent<AIController>()->m_aiConfig.velocity.GetZ() * vectorInOtherDirection.GetX();
		isLeft = dot > 0;
	}
	m_aiConfig.avoidanceDirection = isLeft;

	//Calculate a right angle of the vector between us
	resultVector = isLeft ? Vector3(-vectorInOtherDirection.GetZ(), 0.0f, vectorInOtherDirection.GetX()) : Vector3(vectorInOtherDirection.GetZ(), 0.0f, -vectorInOtherDirection.GetX());
	resultVector.Normalize();

	//Move it out based on our radius + theirs
	AABB closestFixtureBoundingBox = closestFixture->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
	Vector3 closestFixtureRadius = (closestFixtureBoundingBox.getMax() - closestFixtureBoundingBox.getMin()) * (1.0f / 2.0f);

	resultVector.Multiply((gameObjRadius.GetX() + gameObjRadius.GetZ()) / 2.0f + (closestFixtureRadius.GetX() + closestFixtureRadius.GetZ()) / 2.0f);

	//Steer torwards it, increasing force based on how close we are
	return SteeringBehaviourSeek(resultVector) * (1.0f / minFraction);
}

Vector3 AIController::SteeringBehaviourAlignment()
{
	GameObject* gameObj = GetOwner();

	Vector3 velocity = m_aiConfig.velocity;
	Vector3 averageHeading = Vector3(0.0f, 0.0f, 0.0f);
	int neighboursCount = 0;

	//for each of our neighbours (including ourself)

	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		int distance = (gameObj->GetPosition() - itr->GetPosition()).Length();

		//That are within the max distance and are moving
		if (distance < m_aiConfig.maxCohesion && velocity.Length() > 0) {
			//Sum up our headings
			averageHeading = averageHeading + velocity.Normalize();
			neighboursCount++;
		}
	}

	if (neighboursCount == 0) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//Divide to get the average heading
	averageHeading = averageHeading * (1.0f / neighboursCount);

	//Steer towards that heading
	Vector3 desired = averageHeading * m_aiConfig.maxSpeed;
	Vector3 force = desired - velocity;
	return force * (m_aiConfig.maxForce / m_aiConfig.maxSpeed);
}

Vector3 AIController::SteeringBehaviourSeek(const Vector3& centerOfMass)
{
	GameObject* gameObj = GetOwner();

	const Vector3 velocity = m_aiConfig.velocity;

	//Desired change of location
	Vector3 desired = m_aiConfig.flowField->getDestination() - gameObj->GetPosition();

	//Desired velocity (move there at maximum speed)
	desired = desired * (m_aiConfig.maxSpeed / desired.Length());

	//The velocity change we want
	Vector3 force = desired - velocity;

	//Convert to a force
	return force * (m_aiConfig.maxForce / m_aiConfig.maxSpeed);
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
			float distance = (gameObj->GetPosition() - itr->GetPosition()).Length();
			if (distance < m_aiConfig.maxCohesion) {
				//sum up the position of our neighbours
				centerOfMass = centerOfMass + itr->GetPosition();
				neighboursCount++;
			}
		}
	}

	if (neighboursCount == 1) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//Get the average position of ourself and our neighbours
	centerOfMass = centerOfMass * (1.0f / neighboursCount);

	//seek that position
	return SteeringBehaviourSeek(centerOfMass);
}

Vector3 AIController::SteeringBehaviourSeparation()
{
	GameObject* gameObj = GetOwner();

	Vector3 totalForce = Vector3(0.0f, 0.0f, 0.0f);
	int neighboursCount = 0;

	for (auto itr : *GameWorld::GetInstance()->GetGameObjectList())
	{
		if (itr != gameObj)
		{
			float distance = (gameObj->GetPosition() - itr->GetPosition()).Length();

			if (distance < m_aiConfig.minSeperation && distance > 0.0f)
			{
				AABB boundingBox = gameObj->GetComponent<MeshComponent>()->m_pMeshData->GetBoundingBox();
				Vector3 radius = (boundingBox.getMax() - boundingBox.getMin()) * (1.0f / 2.0f);

				Vector3 pushForce = gameObj->GetPosition() - itr->GetPosition();
				totalForce = totalForce + (pushForce * ((radius.GetX() + radius.GetZ() / 2.0f)));
				neighboursCount++;
			}
		}
	}

	if (neighboursCount == 0) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//Normalise the force back down and then back up based on the maximum force
	totalForce = totalForce * (1 / neighboursCount);
	return totalForce * m_aiConfig.maxForce;
}


void AIController::UpdateCamera()
{
	//Update Camera
	CameraComponent* camera = GetOwner()->GetComponent<DE::CameraComponent>();

	if (camera != nullptr)
	{
		const float owner_y = GetOwner()->GetPosition().GetY();
		const Vector3 camLocalPos = camera->GetLocalPosition();
		static int initLocalY = camLocalPos.GetY();

		camera->SetLocalPosition(Vector3(
			camLocalPos.GetX(),
			LookUpHeight(camera->GetPosition()) - owner_y + initLocalY,
			camLocalPos.GetZ()
		));
	}
}

void AIController::Update(float deltaTime)
{
	//Update the velocity;
	m_aiConfig.velocity = m_aiConfig.flowField->getDirection(GetOwner()->GetPosition());

	/*
	//Work out our behaviours
	Vector3 seek = SteeringBehaviourSeek(GetOwner(), m_aiConfig.flowField->getDestination());
	Vector3 separation = SteeringBehaviourSeparation(GetOwner());
	Vector3 cohesion = SteeringBehaviourCohesion(GetOwner());
	Vector3 alignment = SteeringBehaviourAlignment(GetOwner());

	//Combine them to come up with a total force to apply, decreasing the effect of cohesion
	m_aiConfig.velocity = (seek + separation + (cohesion * 0.1f) + alignment) * deltaTime;
	*/

	m_aiConfig.velocity = SteeringBehaviourAvoid();

	const float speed = m_aiConfig.velocity.Length();
	if (speed > m_aiConfig.maxSpeed)
	{
		m_aiConfig.velocity = m_aiConfig.velocity * (m_aiConfig.maxSpeed / speed);
	}

	Move(m_aiConfig.velocity * deltaTime);
	UpdateCamera();
}

void AIController::Move(const Vector3& vTrans)
{
	// *GetOwner()->GetTransform() *= DirVecToMatrix(LookUpDirection(GetOwner()->GetPosition()));

	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	GetOwner()->TransformBy(trans);
}

};
