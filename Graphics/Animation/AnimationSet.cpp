#include "AnimationSet.h"

AnimationSet::AnimationSet(const float currTime, const bool active) :
	m_currTime(currTime), m_active(active)
{

}

AnimationSet::~AnimationSet()
{
}

void AnimationSet::AddAnimation(Animation* animation)
{
	m_animations.insert(std::make_pair(animation->getNodeName(), animation));
}

void AnimationSet::removeAnimation(const std::string name)
{
	m_animations.erase(name);
}

Animation* AnimationSet::getAnimation(const std::string name)
{
	auto t = m_animations.find(name);

	if (t == m_animations.end()) {
		return nullptr;
	}
	else {
		return t->second;
	}

	return nullptr;
}

int AnimationSet::getNumAnimations() const
{
	return m_animations.size();
}

void AnimationSet::update(const float delta_time)
{
	m_currTime = m_currTime + delta_time;

	for (auto itr : m_vAnimations)
	{
		itr->update(delta_time);
	}

	if (m_currTime > m_duration) {
		//m_active = false;
	}
}

float AnimationSet::getCurrTime() const
{
	return m_currTime;
}

void AnimationSet::setCurrTime(const float currTime)
{
	const int delta_time = currTime - m_currTime;
	m_currTime = currTime;

	for (
		std::unordered_map<std::string, Animation*>::iterator it = m_animations.begin();
		it != m_animations.end();
		++it
	) {
		it->second->update(delta_time);
	}
}

float AnimationSet::getDuration() const
{
	return m_duration;
}

void AnimationSet::setDuration(const float duration)
{
	m_duration = duration;
}

bool AnimationSet::isActive() const
{
	return m_active;
}

void AnimationSet::setActive(const bool active)
{
	m_active = active;
}