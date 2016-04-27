#include "PlayerGOS.h"
#include "DEngine\GameObject\GameObject.h"
#include "DEngine/Graphics/MeshComponent.h"
#include "DEngine/Graphics/Scene/SceneGraph.h"
#include "DEngine/AI/PathFinding/AIController.h"
#include "DEngine/Graphics/Animation/AnimationController.h"
#include "DEngine/Graphics/Animation/AnimationStateMachine.h"
#include "DEngine/Object/MovementController.h"
#include "DEngine/Graphics/ParticleSystem/Emitter.h"
#include "DEngine\Light\PointLightComponent.h"
#include "DEngine\Audio\AudioComponent.h"
#include "PlayerMC.h"


PlayerGOS::PlayerGOS(DE::SpawnConfig* spawnConfig, DE::SpawnConfigType spawnConfigType, DE::Terrain* terrain)
	: DE::GameObjectSpawner(spawnConfig, spawnConfigType, terrain)
{
}


PlayerGOS::~PlayerGOS()
{
}

void PlayerGOS::CopyOverrideComponent(DE::GameObject* gameObj, DE::Component* component)
{
	switch (component->GetID())
	{
		case DE::ComponentID::AI_CONTROLLER:
		{
			DE::Handle hAIController(sizeof(DE::AIController));
			new (hAIController) DE::AIController(((DE::AIController*) component)->m_aiConfig);
			gameObj->AddComponent((DE::Component*) hAIController.Raw());
		}
		break;
	}
}

void PlayerGOS::CopyComponent(DE::GameObject* spawnTarget, DE::GameObject* gameObj, const int componentID)
{
	switch (componentID)
	{
	case DE::ComponentID::MESH:
	{
		DE::Handle hMeshComp(sizeof(DE::MeshComponent));
		new (hMeshComp) DE::MeshComponent(spawnTarget->GetComponent<DE::MeshComponent>()->m_pMeshData);
		gameObj->AddComponent((DE::Component*)hMeshComp.Raw());
		DE::SceneGraph::GetInstance()->AddComponent((DE::MeshComponent*)hMeshComp.Raw());
	}
	break;
	case DE::ComponentID::RIGIDBODY:
	{
		DE::Handle hAABB(sizeof(DE::AABB));
		new (hAABB) DE::AABB(spawnTarget->GetComponent<DE::MeshComponent>()->m_pMeshData->GetBoundingBox());
		DE::AABB* aabb = (DE::AABB*) hAABB.Raw();
		aabb->setType(DE::typeAABB);
		gameObj->AddComponent(aabb);
	}
	break;
	case DE::ComponentID::AI_CONTROLLER:
	{
		DE::Handle hAIController(sizeof(DE::AIController));
		new (hAIController) DE::AIController(spawnTarget->GetComponent<DE::AIController>()->m_aiConfig);
		gameObj->AddComponent((DE::Component*) hAIController.Raw());

	}
	break;
	case DE::ComponentID::POINTLIGHT:
	{
		DE::Handle hPointLight(sizeof(DE::PointLightComponent));
		new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 3.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4, 2);
		gameObj->AddComponent((DE::Component*) hPointLight.Raw());
	}
	break;
	case DE::ComponentID::SKELETON:
	{
		gameObj->AddComponent(spawnTarget->GetComponent<DE::Skeleton>());
	}
	break;
	case DE::ComponentID::AUDIO_COMPONENT:
	{
		DE::Handle hAudio(sizeof(DE::AudioComponent));
		new (hAudio) DE::AudioComponent(spawnTarget->GetComponent<DE::AnimationStateMachine>());
		gameObj->AddComponent((DE::Component*) hAudio.Raw());
	}
	break;
	}
}

int PlayerGOS::Spawn(DE::GameObject*& gameObj)
{
	gameObj = new DE::GameObject[2];

	Player* player = (Player*)m_spawnConfig->spawnTarget;
	DE::GameObject* spawnPlayer = &gameObj[0];

	for (auto itr : m_vOverrideComponents)
	{
		CopyOverrideComponent(spawnPlayer, itr);
	}

	DE::MeshComponent* meshComponent = player->GetComponent<DE::MeshComponent>();
	if (meshComponent && FindOverrideComponent(meshComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, meshComponent->GetID());
	}

	DE::AABB* aabbComponent = player->GetComponent<DE::AABB>();
	if (aabbComponent && meshComponent && FindOverrideComponent(aabbComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, aabbComponent->GetID());
	}

	DE::AIController* aiComponent = player->GetComponent<DE::AIController>();
	if (aiComponent && FindOverrideComponent(aiComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, aiComponent->GetID());
	}

	DE::PointLightComponent* plComponent = player->GetComponent<DE::PointLightComponent>();
	if (plComponent && FindOverrideComponent(plComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, plComponent->GetID());
	}

	DE::Skeleton* sklComponent = player->GetComponent<DE::Skeleton>();
	if (sklComponent && FindOverrideComponent(sklComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, sklComponent->GetID());
	}

	DE::AudioComponent* audioComponent = player->GetComponent<DE::AudioComponent>();
	DE::AnimationStateMachine* asmComponent = player->GetComponent<DE::AnimationStateMachine>();
	if (audioComponent && asmComponent  && FindOverrideComponent(audioComponent->GetID()) == -1)
	{
		CopyComponent(player, spawnPlayer, audioComponent->GetID());
	}

	// Weapon
	DE::GameObject* spawnWeapon = &gameObj[1];


	DE::MeshComponent* w_meshComponent = player->m_Weapon->GetComponent<DE::MeshComponent>();
	if (w_meshComponent)
	{
		CopyComponent(player->m_Weapon, spawnWeapon, w_meshComponent->GetID());
	}

	DE::AABB* w_aabbComponent = player->m_Weapon->GetComponent<DE::AABB>();
	if (w_aabbComponent && w_meshComponent)
	{
		CopyComponent(spawnWeapon, spawnWeapon, w_aabbComponent->GetID());
	}

	DE::Transform* w_transformComponent = spawnWeapon->GetComponent<DE::Transform>();
	if (w_transformComponent && sklComponent)
	{
		w_transformComponent->AttachToJoint(spawnPlayer->GetComponent<DE::Skeleton>(), 31);
		w_transformComponent->AttachTo(spawnPlayer->m_pTransform);
	}

	/*
	std::string id = "flare" + std::to_string(m_accuSpawnNum);
	DE::Handle hEmitter(sizeof(DE::Emitter));
	new (hEmitter) DE::Emitter(const_cast<char*>(id.c_str()), DE::Emitter::TORCH_FLAME, 2.0f, DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 1.0f, 0.0f));
	spawnWeapon->AddComponent((DE::Component*) hEmitter.Raw());
	*/

	return 2;
}
