// Game include
#include "Boss.h"
#include "AIBehavior.h"
#include "BossASM.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Light\PointLightComponent.h"
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"
#include "DEngine\Graphics\HUD\HUD.h"
#include "DEngine\Event\EventQueue.h"

#include <Windows.h>

Boss::Boss(Player* player)
	: DE::GameObject()
	, m_fHP(1000.0f)
	, m_Player(player)
	, m_bHitPlayer(false)
{
	DE::Handle hMeshComponent(sizeof(DE::MeshComponent));
	new (hMeshComponent) DE::MeshComponent("mutant", DE::eMeshType::SKELETAL_MESH);
	AddComponent((DE::Component*) hMeshComponent.Raw());

	DE::Handle hAABB(sizeof(DE::AABB));
	// hard code bounding box for the boss body
	new (hAABB) DE::AABB(DE::Vector3(-0.4f, -0.2f, -0.4f), DE::Vector3(0.6f, 1.8f, 0.6f));
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
	new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 6.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 6, 4);
	AddComponent((DE::Component*) hPointLight.Raw());

	// Left hand
	m_pLeftHand = new DE::GameObject();
	DE::Handle hLeftHandAABB(sizeof(DE::AABB));
	//new (hLeftHandAABB) DE::AABB(DE::Vector3(0, 0.4, 0.6), DE::Vector3(0.1, 0.7, 0.9));
	new (hLeftHandAABB) DE::AABB(DE::Vector3(-0.1, -0.1, -0.1), DE::Vector3(0.2, 0.2, 0.1));
	m_pLeftHand->AddComponent((DE::Component*) hLeftHandAABB.Raw());
	DE::Handle hLeftHandEmitter(sizeof(DE::Emitter));
	new (hLeftHandEmitter) DE::Emitter("lefthand", DE::Emitter::TORCH_FLAME, 1.0f, DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 1.0f, 0.0f));
	m_pLeftHand->AddComponent((DE::Component*) hLeftHandEmitter.Raw());
	m_pLeftHand->GetComponent<DE::Transform>()->AttachToJoint(GetComponent<DE::Skeleton>(), 10);
	m_pLeftHand->GetComponent<DE::Transform>()->AttachTo(m_pTransform);

	

	// Right hand
	m_pRightHand = new DE::GameObject();
	DE::Handle hRightHandAABB(sizeof(DE::AABB));
	new (hRightHandAABB) DE::AABB(DE::Vector3(-0.1, -0.1, -0.1), DE::Vector3(0.2, 0.2, 0.1));
	m_pRightHand->AddComponent((DE::Component*) hRightHandAABB.Raw());
	DE::Handle hRightHandEmitter(sizeof(DE::Emitter));
	new (hRightHandEmitter) DE::Emitter("righthand", DE::Emitter::TORCH_FLAME, 1.0f, DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 1.0f, 0.0f));
	m_pRightHand->AddComponent((DE::Component*) hRightHandEmitter.Raw());
	m_pRightHand->GetComponent<DE::Transform>()->AttachToJoint(GetComponent<DE::Skeleton>(), 31);
	m_pRightHand->GetComponent<DE::Transform>()->AttachTo(m_pTransform);

}


void Boss::Update(float deltaTime)
{
	DE::GameObject::Update(deltaTime);
	DE::DEBUG_RENDERER::GetInstance()->boosHPWidth = 800.0f * m_fHP / 1000.0f;
	m_fAttackTime += deltaTime;

	// Check attack collision
	if (m_Player && !m_bHitPlayer && m_Player->m_fHP >= 10.0f)
	{
		if (m_eState == PUNCHING || m_eState == JUMPATTACKING)
		{
			if (m_pLeftHand->isCollided((GameObject*)m_Player) || m_pRightHand->isCollided((GameObject*)m_Player))
			{
				if (m_Player->GetState() != Player::DOGDING && m_eAttacked == WAITING)
				{

					m_bHitPlayer = true;
					m_Player->m_fHP -= 10.0f;
					m_eAttacked == ATTACKED;
					DE::Handle h(sizeof(Player_Impact_START_Event));
					new (h) Player_Impact_START_Event;
					DE::EventQueue::GetInstance()->Add(h, DE::GAME_EVENT);
				}
			}
		}
	}
}

Boss::~Boss()
{
}
