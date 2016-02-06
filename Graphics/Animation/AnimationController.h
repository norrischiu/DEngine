#pragma once

#include <string>
#include "Skeleton.h"
#include "AnimationSet.h"
#include <unordered_map>

class AnimationController
{
public:
	AnimationController(Skeleton* skeleton);
	~AnimationController();

	void addAnimationSet(const std::string name, const AnimationSet& animationSet);
	void setAnimationSet(const std::unordered_map<std::string, AnimationSet>& animationSet);
	void removeAnimationSet(const std::string name);
	AnimationSet* findAnimationSet(const std::string name);
	Animation* findAnimation(const std::string set_name, const std::string animation_name);
	void setActiveAnimationSet(const std::string name, const bool active);
	bool isAnimationSetActive(const std::string name);
	bool triggerAnimation(const std::string set_name, const float currTime);
	int getNumAnimationSets() const;
	int getNumAnimations(const std::string name);

	void update(const float delta_time);

private:
	std::unordered_map<std::string, AnimationSet> m_animationSets;
	Skeleton* m_skeleton;
};
