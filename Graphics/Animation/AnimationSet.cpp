#include "AnimationSet.h"

AnimationSet::AnimationSet()
{
}


AnimationSet::AnimationSet(const float currTime, const bool active = true) :
	m_currTime(currTime), m_active(active)
{

}

AnimationSet::~AnimationSet()
{
}

void AnimationSet::addAnimation(const std::string name, const Animation& animation)
{
	std::pair <std::string, Animation> t_animation = std::make_pair(name, animation);
	m_animations.insert(t_animation);
}

void AnimationSet::removeAnimation(const std::string name)
{
	m_animations.erase(name);
}


Animation* AnimationSet::findAnimation(const std::string name)
{
	auto t = m_animations.find(name);

	if (t == m_animations.end()) {
		return nullptr;
	} else {
		return &t->second;
	}

	return nullptr;
}

void AnimationSet::update(const float time)
{
	m_currTime = time;
}

float AnimationSet::getCurrTime()
{
	return m_currTime;
}

int AnimationSet::getNumAnimations() const
{
	return m_animations.size();
}

bool AnimationSet::isActive() const
{
	return m_active;
}

void AnimationSet::setActive(const bool active)
{
	m_active = active;
}