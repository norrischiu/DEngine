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

	static const int ComponentID = ComponentID::ANIMATION_CONTROLLER;

	AnimationController(Skeleton* skeleton);
	~AnimationController();

	void addAnimationSet(const std::string name, const AnimationSet& animationSet, const BlendMode blendMode = ADDICTIVE_BLENDING);

	void CreateAnimationSets(const char* fileName);

	void removeAnimationSet(const std::string name);
	void setAnimationSet(const std::unordered_map<std::string, AnimationSet>& animationSets);
	
	std::unordered_map<std::string, AnimationSet>* getAnimationSets();
	AnimationSet* getAnimationSet(const std::string name);
	Animation* getAnimation(const std::string set_name, const std::string animation_name);

	Skeleton* getSkeleton();
	void setSkeleton(Skeleton* const skeleton);

	int getNumAnimationSets() const;
	int getNumAnimations(const std::string name);

	bool isAnimationSetActive(const std::string name);
	void setActiveAnimationSet(const std::string name, const bool active);
	
	bool triggerAnimation(const std::string set_name, const float currTime);

	void setBlending(const std::vector<std::string> clips, const BlendMode blendMode);

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	std::unordered_map<std::string, AnimationSet> m_animationSets;
	std::unordered_map<BlendMode, std::vector<std::vector<std::string>>> m_blending;
	Skeleton* m_skeleton;
	bool m_bPlaying;
};
