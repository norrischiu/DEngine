// PlayerASM.cpp

// Game include
#include "PlayerASM.h"
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

	DE::Handle hBTree(sizeof(DE::BlendTree));
	new (hBTree) DE::BlendTree();
	((DE::BlendTree*)hBTree.Raw())->m_eBlendType = DE::BlendTree::eBlendType::ONE_D;
	((DE::BlendTree*)hBTree.Raw())->m_vClipnames.push_back("walk");
	((DE::BlendTree*)hBTree.Raw())->m_vWeightings.push_back(0);
	((DE::BlendTree*)hBTree.Raw())->m_vClipnames.push_back("strafe_right");
	((DE::BlendTree*)hBTree.Raw())->m_vWeightings.push_back(1);
	((DE::BlendTree*)hBTree.Raw())->m_vClipnames.push_back("walk_back");
	((DE::BlendTree*)hBTree.Raw())->m_vWeightings.push_back(2);
	((DE::BlendTree*)hBTree.Raw())->m_vClipnames.push_back("strafe_left");
	((DE::BlendTree*)hBTree.Raw())->m_vWeightings.push_back(3);
	((DE::BlendTree*)hBTree.Raw())->m_eBlendType = DE::BlendTree::eBlendType::ONE_D;
	((DE::BlendTree*)hBTree.Raw())->m_fBlendFactor = -1.0f;
	AddState("WALK", (DE::BlendTree*) hBTree.Raw());

	AddTransistion("IDLE", "WALK", 1, 0.3f);
	AddTransistion("WALK", "IDLE", 1, 0.3f);
	AddTransistion("IDLE", "ATTACK", 1, 0.1f);
	AddTransistion("ATTACK", "IDLE", 1, 0.1f);
	AddTransistion("WALK", "ATTACK", 1, 0.1f);
	AddTransistion("ATTACK", "WALK", 1, 0.1f);
}

void PlayerASM::Update(float deltaTime)
{
	if (m_bInTrasition && (m_fTransitionTime + deltaTime > m_pCurrTransition->m_fDuration) && (strcmp(m_pPrevState->m_sName, "WALK") == 0) && (strcmp(m_pCurrState->m_sName, "IDLE") == 0))
	{
		m_pPrevState->m_BlendTree->m_fBlendFactor = -1.0f;
	}

	DE::AnimationStateMachine::Update(deltaTime);

	while (!DE::EventQueue::GetInstance()->Empty(DE::GAME_EVENT))
	{
		DE::Handle hEvt = DE::EventQueue::GetInstance()->Front(DE::GAME_EVENT);
		HandleEvent(hEvt);
		DE::EventQueue::GetInstance()->Pop(DE::GAME_EVENT);
	}
}

void PlayerASM::HandleEvent(DE::Handle hEvt)
{
	if (!m_bInTrasition)
	{
		DE::Event* pEvt = (DE::Event*) hEvt.Raw();
		float forwardDot, rightDot;
		switch (pEvt->m_ID)
		{
		case GameEventID::Player_Walk_START_Event:
			ChangeStateTo("WALK");
			forwardDot = m_pOwner->GetTransform()->GetForward().Dot(((Player_Walk_START_Event*)pEvt)->m_vDir);
			rightDot = m_pOwner->GetTransform()->GetRight().Dot(((Player_Walk_START_Event*)pEvt)->m_vDir);
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
			break;
		case GameEventID::Player_Walk_END_Event:
			ChangeStateTo("IDLE");
			break;
		case GameEventID::Player_Attack_1_START_Event:
			ChangeStateTo("ATTACK");
			break;
		case GameEventID::Player_Attack_1_END_Event:
			((Player*)m_pOwner)->SetState(Player::NOT_ATTACKING);
			ChangeStateTo(m_pPrevState->m_sName);
			break;
		case DE::EngineEventID::Animation_END_Event:
			if (strcmp(m_pCurrState->m_sName, "ATTACK") == 0)
			{
				DE::Handle hEvt(sizeof(Player_Attack_1_END_Event));
				new (hEvt) Player_Attack_1_END_Event;
				DE::EventQueue::GetInstance()->Add(hEvt, DE::GAME_EVENT);
			}
			break;
		}
	}
}
