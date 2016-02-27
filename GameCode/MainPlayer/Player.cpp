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
	SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	AddComponent(new PlayerMC());

	// Create animation controller and skeleton
	Skeleton* skel = new Skeleton("maria");
	AnimationController* animController = new AnimationController(skel);
	animController->CreateAnimationSets("maria_idle");
	animController->CreateAnimationSets("maria_attack");
	animController->getAnimationSet("idle")->SetLooping(true);
	animController->getAnimationSet("idle")->setActive(true);

	std::vector<std::string> names;
	names.push_back(std::string("idle"));
	names.push_back(std::string("attack1"));
	animController->setBlending(names, AnimationController::BlendMode::CROSS_FADE_BLENDING);

	AddComponent(skel);
	AddComponent(animController);

	// Create animation state machine
	PlayerASM* playerAsm = new PlayerASM(animController);
	AddComponent(playerAsm);
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
