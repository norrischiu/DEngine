#include "Player.h"

// Game include
#include "PlayerASM.h"
#include "PlayerMC.h"

// Engine include
#include "DEngine\Memory\Handle.h"
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Graphics\MeshData.h"
#include "DEngine\Graphics\Animation\Skeleton.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"
#include "DEngine\GameObject\GameObject.h"
#include "DEngine\Object\CameraComponent.h"
#include "DEngine\Object\MovementController.h"
#include "DEngine\Light\PointLightComponent.h"
#include "DEngine\Audio\AudioSystem.h"
#include "DEngine\Audio\AudioComponent.h"
#include "DEngine\Math\simdmath.h"

Player::Player()
	: DE::GameObject()
	, m_fHP(100.0f)
{
	DE::Handle hMeshComponent(sizeof(DE::MeshComponent));
	new (hMeshComponent) DE::MeshComponent("maria", DE::eMeshType::SKELETAL_MESH);
	AddComponent((DE::Component*) hMeshComponent.Raw());

	DE::Handle hAABB(sizeof(DE::AABB));
	new (hAABB) DE::AABB(((DE::MeshComponent*) hMeshComponent.Raw())->m_pMeshData->GetBoundingBox());
	AddComponent((DE::Component*) hAABB.Raw());

	DE::Handle hMC(sizeof(PlayerMC));
	new (hMC) PlayerMC();
	AddComponent((DE::Component*) hMC.Raw());

	DE::Handle hSkel(sizeof(DE::Skeleton));
	new (hSkel) DE::Skeleton("maria");
	AddComponent((DE::Component*) hSkel.Raw());

	DE::Handle hAnimController(sizeof(DE::AnimationController));
	new (hAnimController) DE::AnimationController((DE::Skeleton*) hSkel.Raw());
	AddComponent((DE::Component*) hAnimController.Raw());

	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("maria_walk");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("maria_idle");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("maria_attack1");
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("walk")->SetLooping(true);
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("idle")->SetLooping(true);
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("walk")->SetActive(true);

	DE::Handle hASM(sizeof(PlayerASM));
	new (hASM) PlayerASM((DE::AnimationController*) hAnimController.Raw());
	AddComponent((DE::Component*) hASM.Raw());

	DE::AudioSystem::GetInstance()->AddAudio("walk", L"footstep");
	DE::Handle hAudio(sizeof(DE::AudioComponent));
	new (hAudio) DE::AudioComponent((DE::AnimationStateMachine*) hASM.Raw());
	AddComponent((DE::Component*) hAudio.Raw());

	// Small surrounding light
	DE::Handle hPointLight(sizeof(DE::PointLightComponent));
	new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 3.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4, 2);
	AddComponent((DE::Component*) hPointLight.Raw());

	// Follow camera
	/*
	DE::Handle hCamera(sizeof(DE::CameraComponent));
	new (hCamera) DE::CameraComponent(DE::Vector3(0.0f, 3.0f, -8.0f), DE::Vector3(0.0f, 2.5f, 0.0f), DE::Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);
	AddComponent((DE::Component*) hCamera.Raw());
	*/

	// Weapon
	m_Weapon = new DE::GameObject();

	DE::Handle hWeaponMeshComponent(sizeof(DE::MeshComponent));
	new (hWeaponMeshComponent) DE::MeshComponent("sword", DE::eMeshType::STANDARD_MESH);
	m_Weapon->AddComponent((DE::Component*) hWeaponMeshComponent.Raw());

	DE::Handle hWeaponAABB(sizeof(DE::AABB));
	new (hWeaponAABB) DE::AABB(m_Weapon->GetComponent<DE::MeshComponent>()->m_pMeshData->GetBoundingBox());
	m_Weapon->AddComponent((DE::Component*) hWeaponAABB.Raw());

	m_Weapon->GetComponent<DE::Transform>()->AttachToJoint(GetComponent<DE::Skeleton>(), 31);
	m_Weapon->GetComponent<DE::Transform>()->AttachTo(m_pTransform);

	// Fire
	DE::Handle hEmitter(sizeof(DE::Emitter));
	new (hEmitter) DE::Emitter("flare", DE::Emitter::TORCH_FLAME, 2.0f, DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 1.0f, 0.0f));
	m_Weapon->AddComponent((DE::Component*) hEmitter.Raw());
}

void Player::Update(float deltaTime)
{
	DE::GameObject::Update(deltaTime);
	m_Weapon->TransformBy(*m_pTransform->m_mWorldTransform);
}
