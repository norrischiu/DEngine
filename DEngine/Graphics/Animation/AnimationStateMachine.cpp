#include "AnimationStateMachine.h"

namespace DE
{

AnimationStateMachine::AnimationStateMachine(AnimationController* animController)
	: m_pController(animController)
	, m_iNumState(0)
	, m_fTransitionTime(0.0f)
	, m_bInTrasition(false)
{
	m_ID = ComponentID;
	animController->m_bPlaying = true;
}

void AnimationStateMachine::Update(float deltaTime)
{
	if (m_bInTrasition)
	{
		m_fTransitionTime += deltaTime;
		// finished transition
		if (m_fTransitionTime > m_pCurrTransition->m_fDuration)
		{
			m_pController->setActiveAnimationSet(m_pPrevState->m_sClipName, false);
			m_fTransitionTime = 0.0f;
			m_bInTrasition = false;
		}
	}
}

void AnimationStateMachine::ChangeStateTo(const char* stateName)
{
	m_pPrevState = m_pCurrState;
	State* target = (State*) m_mapStates[stateName].Raw();
	Transition* trans = ((Transition*) m_mTransitions[m_pCurrState->m_iIndex][target->m_iIndex].Raw());
	m_pCurrTransition = trans;
	m_bInTrasition = true;
	m_pCurrState = target;

	m_pController->setActiveAnimationSet(m_pCurrState->m_sClipName, true);
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