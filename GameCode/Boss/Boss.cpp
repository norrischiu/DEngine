
#include "Boss.h"
#include "AIBehavior.h"
#include "BossASM.h"
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Light\PointLightComponent.h"
#include "DEngine\Graphics\Animation\AnimationController.h"


Boss::Boss(Player* player)
	: DE::GameObject()
	, m_fHP(0.0f)
	, m_Player(player)
{
	DE::Handle hMeshComponent(sizeof(DE::MeshComponent));
	new (hMeshComponent) DE::MeshComponent("mutant", DE::eMeshType::SKELETAL_MESH);
	AddComponent((DE::Component*) hMeshComponent.Raw());

	DE::Handle hAABB(sizeof(DE::AABB));
	new (hAABB) DE::AABB(((DE::MeshComponent*) hMeshComponent.Raw())->m_pMeshData->GetBoundingBox());
	AddComponent((DE::Component*) hAABB.Raw());

	DE::Handle hAI(sizeof(AIBehavior));
	new (hAI) AIBehavior();
	AddComponent((DE::Component*) hAI.Raw());

	DE::Handle hSkel(sizeof(DE::Skeleton));
	new (hSkel) DE::Skeleton("mutant");
	AddComponent((DE::Component*) hSkel.Raw());

	DE::Handle hAnimController(sizeof(DE::AnimationController));
	new (hAnimController) DE::AnimationController((DE::Skeleton*) hSkel.Raw());
	AddComponent((DE::Component*) hAnimController.Raw());

	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("mutant_walking");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("mutant_idle");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("mutant_jump_attack");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("mutant_punch");
	((DE::AnimationController*) hAnimController.Raw())->CreateAnimationSets("mutant_dying");
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("walk")->SetLooping(true);
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("idle")->SetLooping(true);
	((DE::AnimationController*) hAnimController.Raw())->getAnimationSet("idle")->setActive(true);


	DE::Handle hASM(sizeof(BossASM));
	new (hASM) BossASM((DE::AnimationController*) hAnimController.Raw());
	AddComponent((DE::Component*) hASM.Raw());
	((DE::AnimationController*) hAnimController.Raw())->SetAnimationStateMachine((BossASM*)hASM.Raw());

	// Small surrounding light
	DE::Handle hPointLight(sizeof(DE::PointLightComponent));
	new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 6.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 17, 6.5);
	AddComponent((DE::Component*) hPointLight.Raw());
}


void Boss::Update(float deltaTime)
{
	DE::GameObject::Update(deltaTime);
	//m_Weapon->TransformBy(*m_pTransform->m_mWorldTransform);
}

Boss::~Boss()
{
}
