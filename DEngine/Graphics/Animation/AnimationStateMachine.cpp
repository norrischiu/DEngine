// Engine include
#include "AnimationStateMachine.h"
#include "AnimationController.h"

namespace DE
{

AnimationStateMachine::AnimationStateMachine(AnimationController* animController)
	: m_pController(animController)
	, m_iNumState(0)
	, m_fTransitionTime(0.0f)
	, m_fBlendValue(0.0f)
	, m_bInTrasition(false)
{
	m_ID = ComponentID;
	animController->m_bPlaying = true;
}

void AnimationStateMachine::Update(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	if (m_bInTrasition)
	{
		m_fTransitionTime += deltaTime;
		m_fBlendValue = m_fTransitionTime / m_pCurrTransition->m_fDuration;
		// finished transition
		if (m_fTransitionTime > m_pCurrTransition->m_fDuration)
		{
			SetStateAnimationSetActive(m_pPrevState, false);
			m_fTransitionTime = 0.0f;
			m_bInTrasition = false;
		}
	}
}

void AnimationStateMachine::ChangeStateTo(const char* stateName)
{
	if (strcmp(m_pCurrState->m_sName, stateName) != 0)
	{
		m_pPrevState = m_pCurrState;
		State* target = (State*)m_mapStates[stateName].Raw();
		Transition* trans = ((Transition*)m_mTransitions[m_pCurrState->m_iIndex][target->m_iIndex].Raw());
		m_pCurrTransition = trans;
		m_bInTrasition = true;
		m_fBlendValue = 0.0f;
		m_pCurrState = target;

		SetStateAnimationSetActive(m_pCurrState, true);
	}
}

void AnimationStateMachine::SetStateAnimationSetActive(State* pState, bool isActive)
{
	if (pState->m_bUseBlendTree)
	{
		for (int i = 0; i < pState->m_BlendTree->m_vClipnames.size(); ++i)
		{
			m_pController->setActiveAnimationSet(pState->m_BlendTree->m_vClipnames[i], isActive);
		}
	}
	else
	{
		m_pController->setActiveAnimationSet(pState->m_sClipName, isActive);
	}
}

AnimationStateMachine::~AnimationStateMachine()
{
	for (int i = 0; i < TRANSITION_MAX; ++i)
	{
		for (int j = 0; j < TRANSITION_MAX; ++j)
		{
			m_mTransitions[i][j].Free();
		}
	}

	for (auto itr : m_mapStates)
	{
		itr.second.Free();
	}
}

} // namespace DE