#ifndef ANIMATION_STATE_MACHINE_H_
#define ANIMATION_STATE_MACHINE_H_

// Engine include
#include "Memory\Handle.h"
#include "Object\Component.h"
#include "BlendTree.h"
#include "Utilities\MyHashMap.h"

namespace DE
{

class AnimationController;

#define TRANSITION_MAX 128		// the maximum number of transitions
#define STATE_MAX 64		// the maximum number of states

/*
*	CLASS: AnimationStateMachine
*	Animation State Machine is one of the components that can added to a game object,
*	it stores all the states and transitions, it controls which animation state of a
*	game object should be in
*/
class AnimationStateMachine : public Component
{

public:

	friend AnimationController;

	/********************************************************************************
	*	--- Constructor:
	*	AnimationStateMachine(AnimationController*)
	*	This constructor will construct a animation state machine with a given 
	*	animation controller component
	*
	*	--- Parameters:
	*	@ animController: the pointer to an animation controller component to be 
	*	associated with this state machine
	********************************************************************************/
	AnimationStateMachine(AnimationController* animController);

	/********************************************************************************
	*	--- Destructor:
	*	~AnimationStateMachine()
	*	This destructor will free all memory used by transition and state
	********************************************************************************/
	~AnimationStateMachine();

	/********************************************************************************
	*	--- Function:
	*	GetCurrentClipName()
	*	This function return the animation clip name in currently active state
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ const char*: animation clip name of current state
	********************************************************************************/
	inline const char* GetCurrentClipName()
	{
		return m_pCurrState->m_sClipName;
	}

	/********************************************************************************
	*	--- Function:
	*	GetCurrentBlendTree()
	*	This function return the blend tree in currently active state
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ BlendTree*: pointer to the blend tree of current state
	********************************************************************************/
	inline BlendTree* GetCurrentBlendTree()
	{
		return m_pCurrState->m_BlendTree;
	}

	/********************************************************************************
	*	--- Function:
	*	IsInTransition()
	*	This function check if the state machine is currently transiting
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ bool: True if the state machine is currently in a transition; False if
	*	othe state machine is NOT in a transition (i.e. in a single state)
	********************************************************************************/
	inline bool IsInTransition()
	{
		return m_bInTrasition;
	}
	
	/********************************************************************************
	*	--- Function:
	*	SetAsDefaultState(const char*)
	*	This function will set a state with this name to be the 
	*	default/beginning state
	*
	*	--- Parameters:
	*	@ stateName: the name of the state
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetAsDefaultState(const char* stateName)
	{
		m_pCurrState = (State*) m_mapStates[stateName].Raw();
	}

	/********************************************************************************
	*	--- Function:
	*	ChangeStateTo(const char*)
	*	This function will change to the state with the inputted name, the state
	*	machine will process transition automatically if any is defined between the
	*	current state and target state
	*
	*	--- Parameters:
	*	@ stateName: the name of the target state
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void ChangeStateTo(const char* stateName);

	/********************************************************************************
	*	--- Function:
	*	AddState(const char*, const char*)
	*	This function will add a new state with a single animation clip to the hash
	*	map, memory will be allocated for the state
	*
	*	--- Parameters:
	*	@ stateName: the name of the state
	*	@ clipname: the name of the single animation clip to be referred in this state
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddState(const char* stateName, const char* clipname)
	{
		Handle hState(sizeof(State));
		new (hState) State(stateName, clipname, m_iNumState++);
		m_mapStates.Add(stateName, hState);
	}

	/********************************************************************************
	*	--- Function:
	*	AddState(const char*, BlendTree*)
	*	This function will add a new state with a blend tree to the hash map, memory
	*	will be allocated for the state
	*
	*	--- Parameters:
	*	@ stateName: the name of the state
	*	@ blendTree: the pointer to a blend tree
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddState(const char* stateName, BlendTree* blendTree)
	{
		Handle hState(sizeof(State));
		new (hState) State(stateName, blendTree, m_iNumState++);
		m_mapStates.Add(stateName, hState);
	}

	/********************************************************************************
	*	--- Function:
	*	AddTransistion(const char*, const char*, int, float)
	*	This function will add a new transition to the transition matrix, memory
	*	will be allocated for the transition
	*
	*	--- Parameters:
	*	@ from: the name of the state as transition source
	*	@ to: the name of the state as transition destination
	*	@ blendMode: the blend mode to be used during this transition
	*	@ duration: the duration of this transition
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddTransistion(const char* from, const char* to, int blendMode, float duration = 0.0f)
	{
		Handle hTransition(sizeof(Transition));
		int fromIndex, toIndex;
		fromIndex = ((State*)m_mapStates[from].Raw())->m_iIndex;
		toIndex = ((State*)m_mapStates[to].Raw())->m_iIndex;
		new (hTransition) Transition(fromIndex, toIndex, blendMode, duration);
		m_mTransitions[fromIndex][toIndex] = hTransition;
	}

	/********************************************************************************
	*	--- Pure virtual Function:
	*	HandleEvent(Handle)
	*	This function will handle state change event and reacts respectively. 
	*	It has to be implemented specifically for each game object
	*
	*	--- Parameters:
	*	@ hEvt: the Handle of event
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual bool HandleEvent(Handle hEvt) = 0;

	/********************************************************************************
	*	--- Virtual Function:
	*	Update(float)
	*	This function is inherited via Component
	*	This function will handle transition rule only, checking against its duration
	*
	*	--- Parameters:
	*	@ deltaTime: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void Update(float deltaTime) override;

	static const int							ComponentID = ComponentID::ANIMATION_STATE_MACHINE;		// Component ID

protected:

	/*
	*	STRUCT: AnimationStateMachine::State
	*	State is the information of an animation state, it contains the information 
	*	of animation clip names or a blend tree
	*/
	struct State
	{
		CUSTOM_MEMORY_DEFINE();

		/********************************************************************************
		*	--- Constructor:
		*	State(const char*, const char*, unsigned int)
		*	This constructor will construct a animation state with a single animation clip
		*
		*	--- Parameters:
		*	@ name: the state name
		*	@ clipname: the name of the animation clip to be referred by this state
		*	@ index: the index of this state in the transition array
		********************************************************************************/
		State(const char* name, const char* clipname, unsigned int index)
			: m_sName(name)
			, m_sClipName(clipname)
			, m_iIndex(index)
			, m_bUseBlendTree(false)
		{};

		/********************************************************************************
		*	--- Constructor:
		*	State(const char*, BlendTree*, unsigned int)
		*	This constructor will construct a animation state with a single animation clip
		*
		*	--- Parameters:
		*	@ name: the state name
		*	@ blendTree: the pointer to a blend tree
		*	@ index: the index of this state in the transition array
		********************************************************************************/
		State(const char* name, BlendTree* blendTree, unsigned int index)
			: m_sName(name)
			, m_BlendTree(blendTree)
			, m_iIndex(index)
			, m_bUseBlendTree(true)
		{};

		unsigned int m_iIndex;	// the index in the transition array
		const char* m_sName;	// the state name
		union
		{
			const char* m_sClipName;	// a single animation clip name
			BlendTree* m_BlendTree;		// a pointer to blend tree
		};
		bool m_bUseBlendTree;		// flag on whether this state uses a blend tree
	};

	/*
	*	STRUCT: AnimationStateMachine::Transition
	*	Transition is the information of transition between two animation states, 
	*	it contains the indices to look up the transition matrix, its blend mode,
	*	and the transition duration
	*/
	struct Transition
	{
		CUSTOM_MEMORY_DEFINE();

		/********************************************************************************
		*	--- Constructor:
		*	Transition(unsigned int, unsigned int, int, float)
		*	This constructor will construct a transition between two states
		*
		*	--- Parameters:
		*	@ m_iFrom: the index of the source state in transition matrix
		*	@ m_iTo: the index of the destination state in transition matrix
		*	@ m_iBlendMode: the blend mode
		*	@ m_fDuration: the transition duration
		********************************************************************************/
		Transition(unsigned int from, unsigned int to, int blendMode, float duration)
			: m_iFrom(from)
			, m_iTo(to)
			, m_iBlendMode(blendMode)
			, m_fDuration(duration)
		{};

		unsigned int m_iFrom;	// the index of the source state in transition matrix
		unsigned int m_iTo;		// the index of the destination state in transition matrix
		int m_iBlendMode;		// the blend mode
		float m_fDuration;		// the transition duration
	};

	/********************************************************************************
	*	--- Function:
	*	SetStateAnimationSetActive(State*, bool)
	*	This function will set all animation sets in a state as active or NOT active
	*
	*	--- Parameters:
	*	@ pState: the pointer to a state
	*	@ isActive: True if intends to set animation sets as active; False if intends
	*	to set animation sets as NOT active
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetStateAnimationSetActive(State* pState, bool isActive);

	
	AnimationController*						m_pController;		// pointer to the animation controller associated with this state machine
	Handle										m_mTransitions[TRANSITION_MAX][TRANSITION_MAX];		// transition matrix [from][to]
	MyHashMap<Handle>							m_mapStates;	// hash map of state name to state handle
	unsigned int								m_iNumState;	// number of state	
	State*										m_pCurrState;	// pointer to current state
	State*										m_pPrevState;	// pointer to previous state
	Transition*									m_pCurrTransition;		// pointer to current transition
	float										m_fTransitionTime;		// the accumulated time past in current transition
	bool										m_bInTrasition;		// flag on whether this state machine is currently in a transition
	float										m_fBlendValue;	// blend factor used in transition
};

} // namespace DE

#endif // !ANIMATION_STATE_MACHINE_H_