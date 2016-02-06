#pragma once

#include <string>
#include <unordered_map>
#include "Animation.h"

class AnimationSet
{
public:
	AnimationSet();
	AnimationSet(const float currTime, const bool active = true);
	~AnimationSet();

	void addAnimation(const std::string name, const Animation& animation);
	void removeAnimation(const std::string name);
	Animation* findAnimation(const std::string name);
	std::unordered_map<std::string, Animation>* getAnimations();
	int getNumAnimations() const;

	float getCurrTime() const;
	void setCurrTime(const float time);
	float getDuration() const;
	void setDuration(const float duration);
	bool isActive() const;
	void setActive(const bool active);

	void update(const float delta_time);

private:
	std::unordered_map<std::string, Animation> m_animations;
	float m_currTime;
	float m_duration;
	bool m_active;
};

