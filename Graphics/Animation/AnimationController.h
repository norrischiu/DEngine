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

	static const int ComponentID = ComponentID::ANIMATION_CONTROLLER;

	AnimationController(Skeleton* skeleton);
	~AnimationController();

	void addAnimationSet(const std::string name, const AnimationSet& animationSet);

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

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	std::unordered_map<std::string, AnimationSet> m_animationSets;
	Skeleton* m_skeleton;
};
