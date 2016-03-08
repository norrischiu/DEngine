#include "Player.h"

// Game include
#include "PlayerASM.h"
#include "PlayerMC.h"

// Engine include
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Graphics\MeshData.h"
#include "DEngine\Object\MovementController.h"
#include "DEngine\Graphics\Animation\Skeleton.h"
#include "DEngine\Math\simdmath.h"
#include "DEngine\Object\CameraComponent.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"

Player::Player()
	: DE::GameObject()
	, m_fHP(100.0f)
{
	AddComponent(new DE::MeshComponent("maria", DE::eMeshType::SKELETAL_MESH));
	//AddComponent(new Body(typeAABB));

	SetPosition(DE::Vector3(-4.0f, 0.0f, -4.0f));
	AddComponent(new PlayerMC());
	DE::Skeleton* skel = new DE::Skeleton("maria");
	DE::AnimationController* animController = new DE::AnimationController(skel);
	animController->CreateAnimationSets("maria_walk");
	animController->CreateAnimationSets("maria_idle");
	animController->CreateAnimationSets("maria_attack1");
	animController->getAnimationSet("walk")->SetLooping(true);
	animController->getAnimationSet("idle")->SetLooping(true);
	animController->getAnimationSet("idle")->setActive(true);

	AddComponent(skel);
	AddComponent(animController);
	AddComponent(new PlayerASM(animController));

	AddComponent(new DE::CameraComponent(DE::Vector3(0.0f, 2.0f, -7.0f), DE::Vector3(0.0f, 2.0f, 3.0f), DE::Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
	
}

void Player::Update(float deltaTime)
{
	DE::GameObject::Update(deltaTime);
}
