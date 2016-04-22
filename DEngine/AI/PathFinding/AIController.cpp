#include "AIController.h"
#include "../../GameObject/GameWorld.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/MeshComponent.h"
#include "FlowFieldBuilder.h"

namespace DE
{

AIController::AIController(FlowField* flowField, Terrain* terrain)
	: Component(GetOwner()), m_flowField(flowField), m_terrain(terrain), m_enableAI(false)
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
		LockCurrPosition();
		m_enableAI = true;
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

bool AIController::HasPositionChange(const Vector3& newPos, const Vector3& currPos)
{
	return !(newPos - currPos).iszero();
}

bool AIController::IsSlopeSteep(const Vector3& newPos, const Vector3& currPos)
{
	const Vector3 vec1 = (newPos - currPos).Normalize();
	const Vector3 vec2 = Vector3(vec1.GetX(), 0.0f, vec1.GetZ());
	const float angle = AngleBetween(vec1, vec2) * 180.0f / PI;

	return false && angle > 30.0f;
}

bool AIController::IsPositionOwner(const Vector3& position)
{
	int ownerId = m_flowField->getPositionOwnerId(position);

	return ownerId == GetOwner()->GetGameObjectID();
}

bool AIController::IsNewPositionOwner(const Vector3& newPos)
{
	const Vector3& vTrans = newPos - GetOwner()->GetPosition();

	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	Matrix4 transform = *GetOwner()->GetTransform();
	transform = transform * trans;

	AABB boundingBox = *GetOwner()->GetComponent<AABB>();
	boundingBox.Transform(transform);
	const Vector3& minXYZ = boundingBox.getMin();
	const Vector3& maxXYZ = boundingBox.getMax();

	const int minX = floor(minXYZ.GetX());
	const int maxX = ceil(maxXYZ.GetX());
	const int minZ = floor(minXYZ.GetZ());
	const int maxZ = ceil(maxXYZ.GetZ());

	for (int x = minX; x <= maxX; x++)
	{
		for (int z = minZ; z <= maxZ; z++)
		{
			const Vector3 position = Vector3(x, 0.0f, z);

			if (!IsPositionOwner(position))
			{
				return false;
			}
		}
	}

	return true;
}

bool AIController::IsBlockedByOther(const Vector3& position)
{
	int ownerId = m_flowField->getPositionOwnerId(position);

	return ownerId != -1 && ownerId != GetOwner()->GetGameObjectID();
}

bool AIController::IsNewPositionBlockedByOther(const Vector3& newPos)
{
	const Vector3& vTrans = newPos - GetOwner()->GetPosition();
	
	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	Matrix4 transform = *GetOwner()->GetTransform();
	transform = transform * trans;

	AABB boundingBox = *GetOwner()->GetComponent<AABB>();
	boundingBox.Transform(transform);
	const Vector3& minXYZ = boundingBox.getMin();
	const Vector3& maxXYZ = boundingBox.getMax();

	const int minX = floor(minXYZ.GetX());
	const int maxX = ceil(maxXYZ.GetX());
	const int minZ = floor(minXYZ.GetZ());
	const int maxZ = ceil(maxXYZ.GetZ());

	for (int x = minX; x <= maxX; x++)
	{
		for (int z = minZ; z <= maxZ; z++)
		{
			const Vector3 position = Vector3(x, 0.0f, z);
			
			if (IsBlockedByOther(position))
			{
				return true;
			}
		}
	}

	return false;
}

bool AIController::IsDesintationArrived()
{
	const Vector3 currPos = GetOwner()->GetPosition();
	const Vector3 destination = m_flowField->getDestination();
	
	const Vector3 difference = Vector3(
		floor(abs(currPos.GetX() - destination.GetX())),
		floor(abs(currPos.GetY() - destination.GetY())),
		floor(abs(currPos.GetZ() - destination.GetZ()))
	);

	return difference.iszero();
}

bool AIController::IsActive()
{
	return m_enableAI;
}

void AIController::SetActive(const bool setActive)
{
	m_enableAI = setActive;
}

Vector3 AIController::LookUpDirection(const Vector3& currPos)
{
	return m_flowField->getDirection(currPos);
}

float AIController::LookUpHeight(const Vector3& currPos)
{
	if (m_terrain) {
		return m_terrain->GetHeight(currPos.GetX(), currPos.GetZ());
	}

	return 0.0f;
}

Vector3 AIController::GetNewPosition(const float deltaTime)
{
	Vector3 currPos = GetOwner()->GetPosition();
	Vector3 direction = LookUpDirection(currPos);
	Vector3 newPos = currPos + (direction * deltaTime * 0.5f);

	return newPos;
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

void AIController::SetPositionOwner(const Vector3& position, const int gameObjId)
{
	m_flowField->setPositionOwnerId(position, gameObjId);
}

void AIController::UnLockCurrPosition()
{
	AABB boundingBox = *GetOwner()->GetComponent<AABB>();
	boundingBox.Transform(*GetOwner()->GetTransform());
	const Vector3& minXYZ = boundingBox.getMin();
	const Vector3& maxXYZ = boundingBox.getMax();

	const int minX = floor(minXYZ.GetX());
	const int maxX = ceil(maxXYZ.GetX());
	const int minZ = floor(minXYZ.GetZ());
	const int maxZ = ceil(maxXYZ.GetZ());

	for (int x = minX; x <= maxX; x++)
	{
		for (int z = minZ; z <= maxZ; z++)
		{
			const Vector3 position = Vector3(x, 0.0f, z);
			UnLockPosition(position);
		}
	}
}

void AIController::UnLockPosition(const Vector3& position)
{
	m_flowField->setPositionMovable(position, true);
	SetPositionOwner(GetOwner()->GetPosition(), -1);
}

void AIController::LockCurrPosition()
{
	AABB boundingBox = *GetOwner()->GetComponent<AABB>();
	boundingBox.Transform(*GetOwner()->GetTransform());
	const Vector3& minXYZ = boundingBox.getMin();
	const Vector3& maxXYZ = boundingBox.getMax();

	const int minX = floor(minXYZ.GetX());
	const int maxX = ceil(maxXYZ.GetX());
	const int minZ = floor(minXYZ.GetZ());
	const int maxZ = ceil(maxXYZ.GetZ());

	for (int x = minX; x <= maxX; x++)
	{
		for (int z = minZ; z <= maxZ; z++)
		{
			const Vector3 position = Vector3(x, 0.0f, z);
			LockPosition(position);
		}
	}
}

void AIController::LockPosition(const Vector3& position)
{
	m_flowField->setPositionMovable(position, false);
	SetPositionOwner(GetOwner()->GetPosition(), GetOwner()->GetGameObjectID());
}

void AIController::Update(float deltaTime)
{
	if (!IsActive()) { 
		return; 
	}

	if (IsDesintationArrived()) { 
		return; 
	}

	const Vector3 currPos = GetOwner()->GetPosition();

	UnLockCurrPosition();

	const Vector3 newPos = GetNewPosition(deltaTime);

	LockCurrPosition();

	if (!HasPositionChange(newPos, currPos)) {
		return; 
	}

	if (IsNewPositionBlockedByOther(newPos)) { 
		return; 
	}

	if (IsSlopeSteep(newPos, currPos)) { 
		return; 
	}
	
	Matrix4 rotationMatrix = DirVecToMatrix(LookUpDirection(currPos));

	Move(rotationMatrix, newPos - currPos);
	UpdateCamera();
}

void AIController::Move(Matrix4 rotation, Vector3 vTrans)
{
	UnLockCurrPosition();

	// *GetOwner()->GetTransform() *= rotation;

	Matrix4 trans;
	trans.CreateTranslation(vTrans);
	GetOwner()->TransformBy(trans);

	LockCurrPosition();
}

};
