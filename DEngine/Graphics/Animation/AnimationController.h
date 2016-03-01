#pragma once

#include <string>
#include "Skeleton.h"
#include "AnimationSet.h"
#include "Object\Component.h"
#include <unordered_map>
#include <vector>

class AnimationController : public Component
{

public:

	enum BlendMode {
		ADDICTIVE_BLENDING,
		CROSS_FADE_BLENDING,
		FROZEN_BLENDING
	};

	struct Transition {
		const char* fromClip;
		const char* toClip;
		const BlendMode blendMode;
		const float duration;
		float accuTime;

		Transition(const char* fromClip, const char* toClip, const BlendMode blendMode, const float duration) : fromClip(fromClip), toClip(toClip), blendMode(blendMode), duration(duration), accuTime(0.0f)
		{ }
	};

	static const int ComponentID = ComponentID::ANIMATION_CONTROLLER;

	AnimationController(Skeleton* skeleton);
	~AnimationController();

	void addAnimationSet(std::string name, const AnimationSet& animationSet, const BlendMode blendMode = ADDICTIVE_BLENDING);

	void CreateAnimationSets(const char* fileName);

	void removeAnimationSet(const char* name);
	void setAnimationSet(const std::unordered_map<std::string, AnimationSet>& animationSets);

	std::unordered_map<std::string, AnimationSet>* getAnimationSets();
	AnimationSet* getAnimationSet(const char* name);
	Animation* getAnimation(const char* set_name, const char* animation_name);

	Skeleton* getSkeleton();
	void setSkeleton(Skeleton* skeleton);

	int getNumAnimationSets() const;
	int getNumAnimations(const char* name);

	bool isAnimationSetActive(const char* name);
	void setActiveAnimationSet(const char* name, const bool active);

	bool triggerAnimation(const char* set_name, const float currTime);

	void setBlending(const char* fromClip, const char* toClip, const BlendMode blendMode, const float duration);

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	std::unordered_map<std::string, AnimationSet> m_animationSets;
	std::unordered_map<std::string, Transition> m_transition;
	Skeleton* m_skeleton;
	bool m_bPlaying;
};
