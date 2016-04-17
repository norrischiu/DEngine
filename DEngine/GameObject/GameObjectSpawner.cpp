#include "GameObjectSpawner.h"
#include "../Graphics/MeshComponent.h"
#include "../Graphics/Scene/SceneGraph.h"
#include "../AI/PathFinding/AIController.h"
#include "../Graphics/Animation/AnimationController.h"
#include "../Object/MovementController.h"

namespace DE
{

GameObjectSpawner::GameObjectSpawner(GameObject* spawnTarget, const int spawnNum, const Vector3& spawnPos, const float spawnTimeDelay)
	: m_spawnTarget(spawnTarget), m_spawnNum(spawnNum), m_spawnPos(spawnPos), m_spawnTimeDelay(spawnTimeDelay), m_accuSpawnNum(0), m_accuSpawnTime(0.0f)
{

}


GameObjectSpawner::~GameObjectSpawner()
{
}

void GameObjectSpawner::Update(float deltaTime)
{
	m_accuSpawnTime += deltaTime;

	if (m_accuSpawnNum < m_spawnNum)
	{
		if (m_accuSpawnTime > (m_accuSpawnNum + 1) * m_spawnTimeDelay)
		{
			DE::GameObject* gameObject = new GameObject;

			for (auto itr : *m_spawnTarget->getAllComponents())
			{
				if (itr->GetID() == DE::AIController::ComponentID)
				{
					DE::Handle hAIController(sizeof(DE::AIController));
					new (hAIController) DE::AIController(((DE::AIController*)itr)->m_flowField, ((DE::AIController*)itr)->m_terrain);
					gameObject->AddComponent((DE::Component*) hAIController.Raw());
				}
				else if (itr->GetID() == DE::MeshComponent::ComponentID)
				{
					DE::Handle hMeshComp(sizeof(DE::MeshComponent));
					new (hMeshComp) DE::MeshComponent(m_spawnTarget->GetComponent<DE::MeshComponent>()->m_pMeshData);
					gameObject->AddComponent((DE::Component*)hMeshComp.Raw());
					gameObject->SetPosition(m_spawnPos);

					DE::SceneGraph::GetInstance()->AddComponent((DE::MeshComponent*)hMeshComp.Raw());
				}
				else if (
					itr->GetID() != DE::CameraComponent::ComponentID && 
					itr->GetID() != DE::AnimationController::ComponentID &&
					itr->GetID() != DE::MovementController::ComponentID
				) {
					gameObject->AddComponent(itr);
				}
			}

			m_accuSpawnNum++;
		}
	}
	else
	{

	}

}

}