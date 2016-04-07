#pragma once

#include <string>
#include "Skeleton.h"
#include "AnimationSet.h"
#include "BlendTree.h"
#include "AnimationStateMachine.h"
#include "Object\Component.h"
#include "Math\SQT.h"
#include <vector>

namespace DE
{

class AnimationController : public Component
{

public:

	enum BlendMode {
		ADDICTIVE_BLENDING,
		CROSS_FADE_BLENDING,
		FROZEN_BLENDING
	};

	static const int ComponentID = ComponentID::ANIMATION_CONTROLLER;

	AnimationController(Skeleton* skeleton);
	~AnimationController();

	inline void SetAnimationStateMachine(AnimationStateMachine* pASM)
	{
		m_pASM = pASM;
	}

	void addAnimationSet(const char* set_name, AnimationSet* animationSet);

	void CreateAnimationSets(const char* fileName);

	void removeAnimationSet(const char* set_name);
	void setAnimationSet(const std::unordered_map<std::string, AnimationSet*>& animationSets);

	std::unordered_map<std::string, AnimationSet*>* getAnimationSets();
	AnimationSet* getAnimationSet(const char* set_name);

	Skeleton* getSkeleton();
	void setSkeleton(Skeleton* skeleton);

	int getNumAnimationSets() const;
	int getNumAnimations(const char* set_name);

	bool isAnimationSetActive(const char* set_name);
	void setActiveAnimationSet(const char* set_name, const bool active);

	// Extract the current pose of a joint from a state defined in state machine 
	SQT GetPoseFromState(AnimationStateMachine::State* pState, int jointIndex, float deltaTime);

	// Extract the current pose of a joint from a clip
	SQT GetPoseFromSingleSet(AnimationSet* set, int jointIndex, float deltaTime);

	// Extract the current pose of a joint from a blend tree
	SQT GetPoseFromBlendTree(BlendTree* btree, int jointIndex, float deltaTime);

	// Check if any of the animation set in a state is active
	bool IsStateAnimationSetActive(AnimationStateMachine::State* pState);

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	std::unordered_map<std::string, AnimationSet*>		m_animationSets;
	Skeleton*											m_skeleton;
	bool												m_bPlaying;
	AnimationStateMachine*								m_pASM;
};

};