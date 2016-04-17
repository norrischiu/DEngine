#include "PlayerGOS.h"
#include "DEngine\GameObject\GameObject.h"
#include "DEngine/Graphics/MeshComponent.h"
#include "DEngine/Graphics/Scene/SceneGraph.h"
#include "DEngine/AI/PathFinding/AIController.h"
#include "DEngine/Graphics/Animation/AnimationController.h"
#include "DEngine/Object/MovementController.h"
#include "DEngine/Graphics/ParticleSystem/Emitter.h"
#include "DEngine\Light\PointLightComponent.h"


PlayerGOS::PlayerGOS(Player* player, const int spawnNum, const DE::Vector3& spawnPos, const float spawnTimeDelay)
	: DE::GameObjectSpawner(player, spawnNum, spawnPos, spawnTimeDelay)
{
}


PlayerGOS::~PlayerGOS()
{
}

void PlayerGOS::Update(float deltaTime)
{
	m_accuSpawnTime += deltaTime;

	if (m_accuSpawnNum < m_spawnNum)
	{
		if (m_accuSpawnTime >(m_accuSpawnNum + 1) * m_spawnTimeDelay)
		{
			Player* player = (Player*)m_spawnTarget;

			//Player
			DE::GameObject* spawnPlayer = new DE::GameObject;

			DE::Handle hAIController(sizeof(DE::AIController));
			new (hAIController) DE::AIController(player->GetComponent<DE::AIController>()->m_flowField, player->GetComponent<DE::AIController>()->m_terrain);
			spawnPlayer->AddComponent((DE::Component*) hAIController.Raw());

			DE::Handle hMeshComp(sizeof(DE::MeshComponent));
			new (hMeshComp) DE::MeshComponent(player->GetComponent<DE::MeshComponent>()->m_pMeshData);
			spawnPlayer->AddComponent((DE::Component*)hMeshComp.Raw());
			spawnPlayer->SetPosition(m_spawnPos);
			DE::SceneGraph::GetInstance()->AddComponent((DE::MeshComponent*)hMeshComp.Raw());

			DE::Handle hPointLight(sizeof(DE::PointLightComponent));
			new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 3.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4, 2);
			spawnPlayer->AddComponent((DE::Component*) hPointLight.Raw());

			spawnPlayer->AddComponent(player->GetComponent<DE::Skeleton>());

			// Weapon
			DE::GameObject* spawnWeapon = new DE::GameObject;

			DE::Handle hWeaponMeshComponent(sizeof(DE::MeshComponent));
			new (hWeaponMeshComponent) DE::MeshComponent(player->m_Weapon->GetComponent<DE::MeshComponent>()->m_pMeshData);
			spawnWeapon->AddComponent((DE::Component*) hWeaponMeshComponent.Raw());
			DE::SceneGraph::GetInstance()->AddComponent((DE::MeshComponent*) hWeaponMeshComponent.Raw());

			DE::Handle hWeaponAABB(sizeof(DE::AABB));
			new (hWeaponAABB) DE::AABB(spawnWeapon->GetComponent<DE::MeshComponent>()->m_pMeshData->GetBoundingBox());
			spawnWeapon->AddComponent((DE::Component*) hWeaponAABB.Raw());

			spawnWeapon->GetComponent<DE::Transform>()->AttachToJoint(spawnPlayer->GetComponent<DE::Skeleton>(), 31);
			spawnWeapon->GetComponent<DE::Transform>()->AttachTo(spawnPlayer->m_pTransform);

			// spawnWeapon->AddComponent(player->m_Weapon->GetComponent<DE::Emitter>());

			m_accuSpawnNum++;
		}
	}
	else
	{

	}
}
