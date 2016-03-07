#pragma once

#include <string>
#include <unordered_map>
#include "Animation.h"

namespace DE
{

class AnimationSet
{
public:

	AnimationSet() {};
	AnimationSet(const float currTime, const float duration, const bool active = false, const bool looping = false);
	~AnimationSet();

	void AddAnimation(Animation* animation);

	void removeAnimation(const std::string name);
	Animation* getAnimation(const std::string name);

	int getNumAnimations() const;

	float getCurrTime() const;
	void setCurrTime(const float time);
	float getDuration() const;
	void setDuration(const float duration);
	bool isActive() const;
	void setActive(const bool active);
	bool isLooping();
	void SetLooping(const bool looping);

	void reset();

	void update(const float delta_time);

	std::vector<Animation*> m_vAnimations;

private:
	std::unordered_map<std::string, Animation*> m_animations;
	float m_currTime;
	float m_duration;
	bool m_active;
	bool m_bLooping;
};

};