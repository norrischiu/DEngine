#pragma once

// Engine include
#include "Skeleton.h"
#include "AnimationSet.h"
#include "BlendTree.h"
#include "AnimationStateMachine.h"
#include "Object\Component.h"
#include "Math\SQT.h"

// C++ include
#include <string>
#include <unordered_map>

namespace DE
{

/*
*	CLASS: AnimationController
*	Animation controller is one of the components that can added to a game object,
*	it stores all the animations and calculate the final poses to be passed to
*	matrix palette during rendering
*/
class AnimationController : public Component
{

public:

	/*
	*	ENUM: BlendMode
	*	Blend method between animations
	*/
	enum BlendMode 
	{
		ADDICTIVE_BLENDING,
		CROSS_FADE_BLENDING,
		FROZEN_BLENDING
	};

	/********************************************************************************
	*	--- Constructor:
	*	AnimationController(Skeleton*)
	*	This constructor will construct an animation controller with a given skeleton
	*	component
	*
	*	--- Parameters:
	*	@ skeleton: the pointer to a skeleton component to be associated with this
	*	controller
	********************************************************************************/
	AnimationController(Skeleton* skeleton);

	/********************************************************************************
	*	--- Destructor:
	*	~AnimationController()
	*	This destructor will free all memory used by animation sets added to this
	*	controller
	********************************************************************************/
	~AnimationController();

	/********************************************************************************
	*	--- Function:
	*	SetAnimationStateMachine(AnimationStateMachine*)
	*	This function will associate the animation state machine to this controller
	*
	*	--- Parameters:
	*	@ pASM: the pointer to animation state machine component
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	inline void SetAnimationStateMachine(AnimationStateMachine* pASM)
	{
		m_pASM = pASM;
	}

	/********************************************************************************
	*	--- Function:
	*	setSkeleton(Skeleton*)
	*	This function will associate the skeleton to this controller
	*
	*	--- Parameters:
	*	@ skeleton: the pointer to skeleton component
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	inline void setSkeleton(Skeleton* skeleton)
	{
		m_skeleton = skeleton;
	}

	/********************************************************************************
	*	--- Function:
	*	GetAnimationSetCount()
	*	This function will return the number of animation sets associated in this
	*	controller
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ const int: number of animation sets
	********************************************************************************/
	inline const int GetAnimationSetCount()
	{
		return m_animationSets.size();
	}

	/********************************************************************************
	*	--- Function:
	*	GetAnimationSets()
	*	This function will return the unordered map containing all animation sets
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ std::unordered_map<std::string, AnimationSet*>*: a pointer to the
	*	unordered map
	********************************************************************************/
	inline std::unordered_map<std::string, AnimationSet*> GetAnimationSets()
	{
		return m_animationSets;
	}

	/********************************************************************************
	*	--- Function:
	*	addAnimationSet(const char*, AnimationSet*)
	*	This function will add a single animation set to the controller, identify
	*	by a set name
	*
	*	--- Parameters:
	*	@ setName: name of the animation set
	*	@ animationSet: pointer to the animation set to be added
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void addAnimationSet(const char* setName, AnimationSet* animationSet);

	/********************************************************************************
	*	--- Function:
	*	addAnimationSet(const char*, AnimationSet*)
	*	This function will read an animation definition file from Asset folder and
	*	create all defined animation sets
	*
	*	--- Parameters:
	*	@ fileName: file name of the animation definition file
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void CreateAnimationSets(const char* fileName);

	/********************************************************************************
	*	--- Function:
	*	getAnimationSet(const char*)
	*	This function will return the animation set which matches the name
	*
	*	--- Parameters:
	*	@ setName: the animation set name
	*
	*	--- Return:
	*	@ AnimationSet*: a pointer to the animation set
	********************************************************************************/
	AnimationSet* getAnimationSet(const char* setName);

	/********************************************************************************
	*	--- Function:
	*	setActiveAnimationSet(const char*, const bool)
	*	This function will set a animation set as active or NOT active
	*
	*	--- Parameters:
	*	@ setName: the animation set name
	*	@ bActive: True if intends to set animation set as active; False if intends
	*	to set animation set as NOT active
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void setActiveAnimationSet(const char* setName, const bool bActive);

	/******************************************************************************** 
	*	--- Function:
	*	GetPoseFromState(AnimationStateMachine::State*, int)
	*	This function will extract the current pose of a joint from a state defined 
	*	in state machine, which can be a single or more clips 
	*
	*	--- Parameters:
	*	@ pState: a pointer to a State struct as defined in AnimationStateMachine
	*	@ jointIndex: the joint index of the respective skeleton
	*
	*	--- Return:
	*	@ SQT: the resultant (interpolated/blended) pose of that joint in SQT form
	********************************************************************************/
	SQT GetPoseFromState(AnimationStateMachine::State* pState, const int jointIndex);

	/********************************************************************************
	*	--- Function:
	*	GetPoseFromSingleSet(AnimationSet*, int)
	*	This function will extract the current pose of a joint from a single clip
	*
	*	--- Parameters:
	*	@ set: a pointer to an animation set
	*	@ jointIndex: the joint index of the respective skeleton
	*
	*	--- Return:
	*	@ SQT: the resultant (interpolated) pose of that joint in SQT form
	********************************************************************************/
	SQT GetPoseFromSingleSet(AnimationSet* set, const int jointIndex);

	/********************************************************************************
	*	--- Function:
	*	GetPoseFromBlendTree(BlendTree*, int)
	*	This function will extract the current pose of a joint from a blend tree
	*
	*	--- Parameters:
	*	@ btree: a pointer to a blend tree
	*	@ jointIndex: the joint index of the respective skeleton
	*
	*	--- Return:
	*	@ SQT: the resultant (interpolated and blended) pose of that joint in SQT 
	*	form
	********************************************************************************/
	SQT GetPoseFromBlendTree(BlendTree* btree, const int jointIndex);

	/********************************************************************************
	*	--- Function:
	*	IsStateAnimationSetActive(AnimationStateMachine::State*)
	*	This function will check if any of the animation set in a state is 
	*	currently active
	*
	*	--- Parameters:
	*	@ pState: a pointer to a State struct as defined in AnimationStateMachine
	*
	*	--- Return:
	*	@ bool: True if at least one of the animation sets is active; False if none
	*	is active
	********************************************************************************/
	bool IsStateAnimationSetActive(AnimationStateMachine::State* pState);

	/********************************************************************************
	*	--- Virtual Function:
	*	Update(float)
	*	This function is inherited via Component
	*	This function will update active animations (the current frame) in this
	*	animation controller
	*
	*	--- Parameters:
	*	@ deltaTime: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void Update(float deltaTime) override;

	static const int									ComponentID = ComponentID::ANIMATION_CONTROLLER;	// Component ID

private:

	std::unordered_map<std::string, AnimationSet*>		m_animationSets;	// hash map of animation sets
	Skeleton*											m_skeleton;		// pointer to skeleton component
	AnimationStateMachine*								m_pASM;		// pointer to animation state machine component
	bool												m_bPlaying;		// flag for whether any animation is playing, to fire ANIMATION_END event
};

};