#ifndef ANIMATION_STATE_MACHINE_H_
#define ANIMATION_STATE_MACHINE_H_

// Engine include
#include "Memory\Handle.h"
#include "Object\Component.h"
#include "Graphics\Animation\AnimationController.h"

// C++ include
#include <unordered_map>

namespace DE
{

#define TRANSITION_MAX 128
#define STATE_MAX 64

class AnimationStateMachine : public Component
{

public:
	struct State
	{
		CUSTOM_MEMORY_DEFINE();

		State(const char* name, const char* clipname, unsigned int index)
			: m_sName(name)
			, m_sClipName(clipname)
			, m_iIndex(index)
		{};
		unsigned int m_iIndex;
		const char* m_sName;
		const char* m_sClipName; // TODO: extend to multiple clips
	};

	static const int ComponentID = ComponentID::ANIMATION_STATE_MACHINE;

	// Default constructor
	AnimationStateMachine(AnimationController* animController);

	void SetAsDefaultState(const char* stateName)
	{
		m_pCurrState = (State*) m_mapStates[stateName].Raw();
	}

	void AddState(const char* stateName, const char* clipname)
	{
		Handle hState(sizeof(State));
		new (hState) State(stateName, clipname, m_iNumState++);
		m_mapStates.insert(std::make_pair(stateName, hState));
	}

	void AddTransistion(const char* from, const char* to, int blendMode, float duration = 0.0f)
	{
		Handle hTransition(sizeof(Transition));
		int fromIndex, toIndex;
		fromIndex = ((State*)m_mapStates[from].Raw())->m_iIndex;
		toIndex = ((State*)m_mapStates[to].Raw())->m_iIndex;
		new (hTransition) Transition(fromIndex, toIndex, blendMode, duration);
		m_mTransitions[fromIndex][toIndex] = hTransition;
	}

	// Handle state change events
	virtual void HandleEvent(Handle hEvt) = 0;

	// Inherited via Component, define transition rule here
	virtual void Update(float deltaTime);

	void ChangeStateTo(const char* stateName);

	State* GetCurrentState()
	{
		return m_pCurrState;
	}

	~AnimationStateMachine();

protected:

	struct Transition
	{
		CUSTOM_MEMORY_DEFINE();

		Transition(unsigned int from, unsigned int to, int blendMode, float duration)
			: m_iFrom(from)
			, m_iTo(to)
			, m_iBlendMode(blendMode)
			, m_fDuration(duration)
		{};
		unsigned int m_iFrom;
		unsigned int m_iTo;
		int m_iBlendMode;
		float m_fDuration;
	};

	float										m_fBlendValue;

	std::unordered_map<const char*, Handle>		m_mapStates;

	unsigned int								m_iNumState;

	// Transition matrix [from][to]
	Handle										m_mTransitions[TRANSITION_MAX][TRANSITION_MAX];

	State*										m_pCurrState;

	State*										m_pPrevState;

	Transition*									m_pCurrTransition;

	float										m_fTransitionTime;

	bool										m_bInTrasition;

	AnimationController*						m_pController;
};

} // namespace DE

#endif // !ANIMATION_STATE_MACHINE_H_