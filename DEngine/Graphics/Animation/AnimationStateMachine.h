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

class AnimationStateMachine : public Component
{

public:

	static const int ComponentID = ComponentID::ANIMATION_STATE_MACHINE;

	struct State
	{
		CUSTOM_MEMORY_DEFINE();

		const char* m_sName;
		const char* m_sClipName; // TODO: extend to multiple clips
	};

	struct Transition
	{
		CUSTOM_MEMORY_DEFINE();

		State* m_pFrom;
		State* m_pTo;
		int m_iBlendMode;
		float time;
	};

	// Default constructor
	AnimationStateMachine(AnimationController* animController);

	void SetAsDefaultState(Handle hState)
	{
		m_hCurrState = hState;
	}

	void AddState(Handle hState)
	{
		State* pState = (State*)hState.Raw();
		m_mapStates.insert(std::make_pair(pState->m_sName, hState));
	}

	void AddTransistion(Handle hTransition)
	{

	}

	// Handle state change events
	virtual void HandleEvent(Handle hEvt) = 0;

	// Inherited via Component, define transition rule here
	virtual void Update(float deltaTime) = 0;

	~AnimationStateMachine();

protected:

	float										m_fBlendValue;

	std::unordered_map<const char*, Handle>		m_mapStates;

	Handle										m_hCurrState;

	AnimationController*						m_pController;
};

} // namespace DE

#endif // !ANIMATION_STATE_MACHINE_H_