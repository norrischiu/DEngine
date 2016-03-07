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

Player::Player()
	: GameObject()
	, m_fHP(100.0f)
{
	AddComponent(new MeshComponent("maria", eMeshType::SKELETAL_MESH));
	//AddComponent(new Body(typeAABB));
	SetPosition(Vector3(-4.0f, 0.0f, -4.0f));
	AddComponent(new PlayerMC());
	Skeleton* skel = new Skeleton("maria");
	AnimationController* animController = new AnimationController(skel);
	animController->CreateAnimationSets("maria_walk");
	animController->CreateAnimationSets("maria_idle");
	animController->CreateAnimationSets("maria_attack");
	animController->getAnimationSet("walk")->SetLooping(true);
	animController->getAnimationSet("idle")->SetLooping(true);
	animController->getAnimationSet("idle")->setActive(true);

	AddComponent(skel);
	AddComponent(animController);
	AddComponent(new PlayerASM(animController));
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
