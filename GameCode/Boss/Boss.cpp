// Game include
#include "Boss.h"
#include "AIBehavior.h"
#include "BossASM.h"

// Engine include
#include "DEngine\Graphics\MeshComponent.h"
#include "DEngine\Light\PointLightComponent.h"
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Graphics\HUD\HUD.h"

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
	//new (hAABB) DE::AABB(((DE::MeshComponent*) hMeshComponent.Raw())->m_pMeshData->GetBoundingBox());
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
	new (hPointLight) DE::PointLightComponent(DE::Vector3(0.0f, 6.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 7, 6.5);
	AddComponent((DE::Component*) hPointLight.Raw());

	// Left hand
	m_pLeftHand = new DE::GameObject();
	DE::Handle hLeftHandAABB(sizeof(DE::AABB));
	//new (hLeftHandAABB) DE::AABB(DE::Vector3(0, 0.4, 0.6), DE::Vector3(0.1, 0.7, 0.9));
	new (hLeftHandAABB) DE::AABB(DE::Vector3(-0.1, -0.1, -0.1), DE::Vector3(0.2, 0.2, 0.1));
	m_pLeftHand->AddComponent((DE::Component*) hLeftHandAABB.Raw());

	m_pLeftHand->GetComponent<DE::Transform>()->AttachToJoint(GetComponent<DE::Skeleton>(), 10);
	m_pLeftHand->GetComponent<DE::Transform>()->AttachTo(m_pTransform);

	// Right hand
	m_pRightHand = new DE::GameObject();
	DE::Handle hRightHandAABB(sizeof(DE::AABB));
	new (hRightHandAABB) DE::AABB(DE::Vector3(-0.1, -0.1, -0.1), DE::Vector3(0.2, 0.2, 0.1));
	m_pRightHand->AddComponent((DE::Component*) hRightHandAABB.Raw());

	m_pRightHand->GetComponent<DE::Transform>()->AttachToJoint(GetComponent<DE::Skeleton>(), 31);
	m_pRightHand->GetComponent<DE::Transform>()->AttachTo(m_pTransform);
}


void Boss::Update(float deltaTime)
{
	DE::GameObject::Update(deltaTime);

	// update of HUD bar
	((DE::ProgressBar*)DE::HUD::getInstance()->getHUDElementById("BossHP"))->setProgress(m_fHP / 1000.0f * 100.0f);

	// Check attack collision

	if (m_Player && !m_bHitPlayer)
	{
		if (m_eState == PUNCHING || m_eState == JUMPATTACKING)
		{
			if (m_pLeftHand->isCollided((GameObject*)m_Player) || m_pRightHand->isCollided((GameObject*)m_Player))
			{
				m_bHitPlayer = true;
				m_Player->m_fHP -= 10.0f;
				static wchar_t s[64];
				swprintf(s, 64, L"Unbind: %f\n", m_Player->m_fHP);
				OutputDebugStringW(s);
			}
		}
	}

	// debug drawing
	DE::AABB aabb = *GetComponent<DE::AABB>();
	aabb.Transform(*GetTransform());
	DE::DEBUG_RENDERER::GetInstance()->DRAW_AABB(aabb);

	DE::AABB LHaabb = *m_pLeftHand->GetComponent<DE::AABB>();
	LHaabb.Transform(*m_pLeftHand->GetTransform());
	DE::DEBUG_RENDERER::GetInstance()->DRAW_AABB(LHaabb);

	DE::AABB RHaabb = *m_pRightHand->GetComponent<DE::AABB>();
	RHaabb.Transform(*m_pRightHand->GetTransform());
	DE::DEBUG_RENDERER::GetInstance()->DRAW_AABB(RHaabb);
}

Boss::~Boss()
{
}
