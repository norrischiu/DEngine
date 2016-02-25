#include "AnimationStateMachine.h"

namespace DE
{

AnimationStateMachine::AnimationStateMachine(AnimationController* animController)
	: m_pController(animController)
{
	m_ID = ComponentID;
}

AnimationStateMachine::~AnimationStateMachine()
{
	m_hCurrState.Free();
}

} // namespace DE