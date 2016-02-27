#include "AnimationStateMachine.h"

namespace DE
{

AnimationStateMachine::AnimationStateMachine(AnimationController* animController)
	: m_pController(animController)
	, m_iNumState(0)
	, m_fTransitionTime(0.0f)
{
	m_ID = ComponentID;
}

void AnimationStateMachine::Update(float deltaTime)
{
	if (m_bInTrasition)
	{
		m_fTransitionTime += deltaTime;
		// finished transition
		if (m_fTransitionTime > m_pCurrTransition->m_fDuration)
		{
			m_pController->setActiveAnimationSet("idle", false);
			m_fTransitionTime = 0.0f;
			m_bInTrasition = false;
		}
	}
	if (!m_pController->m_bPlaying)
	{
		m_pController->setActiveAnimationSet("attack1", false);

		State* target = (State*) m_mapStates["IDLE"].Raw();
		assert(target != nullptr);
		Transition* trans = ((Transition*)m_mTransitions[m_pCurrState->m_iIndex][target->m_iIndex].Raw());
		assert(trans != nullptr);
		m_pCurrTransition = trans;
		m_bInTrasition = true;
		m_pCurrState = target;

		m_pController->setActiveAnimationSet("idle", true);
	}

}

void AnimationStateMachine::ChangeStateTo(const char* stateName)
{
	m_pController->setActiveAnimationSet(m_pCurrState->m_sClipName, false);

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