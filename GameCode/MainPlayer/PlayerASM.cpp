// PlayerASM.cpp

// Game include
#include "PlayerASM.h"
#include "PlayerMC.h"
#include "Player.h"
#include "Event\GameEvent.h"

// Engine include
#include "DEngine\Graphics\Animation\AnimationController.h"
#include "DEngine\Graphics\Animation\BlendTree.h"
#include "DEngine\Event\EngineEvent.h"
#include "DEngine\Event\EventQueue.h"
#include "DEngine\Memory\Handle.h"
#include "DEngine\Math\MathHelper.h"
#include <Windows.h>

PlayerASM::PlayerASM(DE::AnimationController * animController)
	: DE::AnimationStateMachine(animController)
{
	AddState("IDLE", "idle");
	SetAsDefaultState("IDLE");
	AddState("ATTACK", "attack1");
	AddState("ATTACK2", "attack2");
	AddState("ATTACK3", "attack3");
	AddState("IMPACT", "impact");

	DE::Handle hWalkBTree(sizeof(DE::BlendTree));
	new (hWalkBTree) DE::BlendTree();
	((DE::BlendTree*)hWalkBTree.Raw())->m_eBlendType = DE::BlendTree::eBlendType::ONE_D;
	((DE::BlendTree*)hWalkBTree.Raw())->m_vClipnames.push_back("walk");
	((DE::BlendTree*)hWalkBTree.Raw())->m_vWeightings.push_back(0);
	((DE::BlendTree*)hWalkBTree.Raw())->m_vClipnames.push_back("strafe_right");
	((DE::BlendTree*)hWalkBTree.Raw())->m_vWeightings.push_back(1);
	((DE::BlendTree*)hWalkBTree.Raw())->m_vClipnames.push_back("walk_back");
	((DE::BlendTree*)hWalkBTree.Raw())->m_vWeightings.push_back(2);
	((DE::BlendTree*)hWalkBTree.Raw())->m_vClipnames.push_back("strafe_left");
	((DE::BlendTree*)hWalkBTree.Raw())->m_vWeightings.push_back(3);
	((DE::BlendTree*)hWalkBTree.Raw())->m_fBlendFactor = -1.0f;
	AddState("WALK", (DE::BlendTree*) hWalkBTree.Raw());

	DE::Handle hDodgeBTree(sizeof(DE::BlendTree));
	new (hDodgeBTree) DE::BlendTree();
	((DE::BlendTree*)hDodgeBTree.Raw())->m_eBlendType = DE::BlendTree::eBlendType::ONE_D;
	((DE::BlendTree*)hDodgeBTree.Raw())->m_vClipnames.push_back("dodge_left");
	((DE::BlendTree*)hDodgeBTree.Raw())->m_vWeightings.push_back(0);
	((DE::BlendTree*)hDodgeBTree.Raw())->m_vClipnames.push_back("dodge_right");
	((DE::BlendTree*)hDodgeBTree.Raw())->m_vWeightings.push_back(1);
	((DE::BlendTree*)hDodgeBTree.Raw())->m_fBlendFactor = -1.0f;
	AddState("DODGE", (DE::BlendTree*) hDodgeBTree.Raw());

	AddTransistion("IDLE", "WALK", 1, 0.2f);
	AddTransistion("WALK", "IDLE", 1, 0.2f);
	AddTransistion("IDLE", "ATTACK", 1, 0.1f);
	AddTransistion("ATTACK", "IDLE", 1, 0.2f);
	AddTransistion("WALK", "ATTACK", 1, 0.1f);
	AddTransistion("ATTACK", "WALK", 1, 0.1f);
	AddTransistion("WALK", "DODGE", 1, 0.3f);
	AddTransistion("DODGE", "WALK", 1, 0.3f);
	AddTransistion("DODGE", "IDLE", 1, 0.3f);
	AddTransistion("IDLE", "DODGE", 1, 0.2f);
	AddTransistion("ATTACK", "ATTACK2", 1, 0.1f);
	AddTransistion("ATTACK2", "IDLE", 1, 0.1f);
	AddTransistion("ATTACK2", "ATTACK3", 1, 0.1f);
	AddTransistion("ATTACK3", "IDLE", 1, 0.1f);
}

void PlayerASM::Update(float deltaTime)
{
	if (m_bInTrasition && (m_fTransitionTime + deltaTime > m_pCurrTransition->m_fDuration) && (strcmp(m_pPrevState->m_sName, "WALK") == 0) && (strcmp(m_pCurrState->m_sName, "IDLE") == 0))
	{
		m_pPrevState->m_BlendTree->m_fBlendFactor = -1.0f;
	}

	DE::AnimationStateMachine::Update(deltaTime);

	int size = DE::EventQueue::GetInstance()->Size(DE::GAME_EVENT);
	for (int i = 0; i < size; ++i)
	{
		DE::Handle hEvt = DE::EventQueue::GetInstance()->Front(DE::GAME_EVENT);
		if (HandleEvent(hEvt))
		{
			DE::EventQueue::GetInstance()->Pop(DE::GAME_EVENT);
		}
		else
		{
			DE::EventQueue::GetInstance()->FrontToBack(DE::GAME_EVENT);
		}
	}
}

bool PlayerASM::HandleEvent(DE::Handle hEvt)
{
	if (!m_bInTrasition)
	{
		DE::Event* pEvt = (DE::Event*) hEvt.Raw();
		float forwardDot, rightDot;
		switch (pEvt->m_ID)
		{
		case GameEventID::Player_Walk_PLAYING_Event:
			ChangeStateTo("WALK");
			forwardDot = m_pOwner->GetTransform()->GetForward().Dot(((Player_Walk_PLAYING_Event*)pEvt)->m_vDir);
			rightDot = m_pOwner->GetTransform()->GetRight().Dot(((Player_Walk_PLAYING_Event*)pEvt)->m_vDir);
			if (forwardDot >= 0.0f)
			{
				if (rightDot >= 0.0f)
				{
					if (m_pCurrState->m_BlendTree->m_fBlendFactor < 0.0f)
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = 1.0f - forwardDot;
					}
					else
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = DE::MathHelper::Lerpf(m_pCurrState->m_BlendTree->m_fBlendFactor, 1.0f - forwardDot, m_fDeltaTime);
					}
				}
				else
				{
					if (m_pCurrState->m_BlendTree->m_fBlendFactor < 0.0f)
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = 3.0f + forwardDot;
					}
					else
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = DE::MathHelper::Lerpf(m_pCurrState->m_BlendTree->m_fBlendFactor, 3.0f + forwardDot, m_fDeltaTime);
					}
				}
			}
			else
			{
				if (rightDot >= 0.0f)
				{
					if (m_pCurrState->m_BlendTree->m_fBlendFactor < 0.0f)
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = 2.0f - rightDot;
					}
					else
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = DE::MathHelper::Lerpf(m_pCurrState->m_BlendTree->m_fBlendFactor, 2.0f - rightDot, m_fDeltaTime);
					}
				}
				else
				{
					if (m_pCurrState->m_BlendTree->m_fBlendFactor < 0.0f)
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = 2.0f + forwardDot;
					}
					else
					{
						m_pCurrState->m_BlendTree->m_fBlendFactor = DE::MathHelper::Lerpf(m_pCurrState->m_BlendTree->m_fBlendFactor, 2.0f + forwardDot, m_fDeltaTime);
					}
				}
			}
			return true;
		case GameEventID::Player_Walk_END_Event:
			ChangeStateTo("IDLE");
			return true;
		case GameEventID::Player_Attack_1_START_Event:
			ChangeStateTo("ATTACK");
			return true;
		case GameEventID::Player_Attack_2_START_Event:
			((Player*)m_pOwner)->m_bHitBoss = false;
			ChangeStateTo("ATTACK2");
			return true;
		case GameEventID::Player_Attack_3_START_Event:
			((Player*)m_pOwner)->m_bHitBoss = false;
			ChangeStateTo("ATTACK3");
			return true;
		case GameEventID::Player_Dodge_START_Event:
			ChangeStateTo("DODGE");
			if (((Player_Dodge_START_Event*)pEvt)->m_vDir.GetX() > 0)
			{
				m_pCurrState->m_BlendTree->m_fBlendFactor = 1.0f;
			}
			else
			{
				m_pCurrState->m_BlendTree->m_fBlendFactor = 0.0f;
			}
			return true;
		case DE::EngineEventID::Animation_END_Event:
			if (strcmp(m_pCurrState->m_sName, "ATTACK") == 0)
			{
				((Player*)m_pOwner)->SetState(Player::IDLING_MOVING);
				m_pOwner->GetComponent<PlayerMC>()->m_ComboSequence[0] = false;
				m_pOwner->GetComponent<PlayerMC>()->m_fComboTime = 0.0f;
				ChangeStateTo(m_pPrevState->m_sName);
				return true;
			}
			else if (strcmp(m_pCurrState->m_sName, "ATTACK2") == 0)
			{
				((Player*)m_pOwner)->SetState(Player::IDLING_MOVING);
				m_pOwner->GetComponent<PlayerMC>()->m_ComboSequence[1] = false;
				m_pOwner->GetComponent<PlayerMC>()->m_fComboTime = 0.0f;
				ChangeStateTo("IDLE");
				return true;
			}
			else if (strcmp(m_pCurrState->m_sName, "ATTACK3") == 0)
			{
				((Player*)m_pOwner)->SetState(Player::IDLING_MOVING);
				m_pOwner->GetComponent<PlayerMC>()->m_ComboSequence[2] = false;
				m_pOwner->GetComponent<PlayerMC>()->m_fComboTime = 0.0f;
				ChangeStateTo("IDLE");
			}
			else if (strcmp(m_pCurrState->m_sName, "DODGE") == 0)
			{
				((Player*)m_pOwner)->SetState(Player::IDLING_MOVING);
				ChangeStateTo(m_pPrevState->m_sName);
				return true;
			}
			return true;
		}
	}
	return false;
}
