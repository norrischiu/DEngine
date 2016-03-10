#include "AnimationSet.h"

namespace DE
{

AnimationSet::AnimationSet(const float currTime, const float duration, const bool active, const bool looping) :
	m_currTime(currTime), m_duration(duration), m_active(active), m_bLooping(looping)
{

}

AnimationSet::~AnimationSet()
{
	for (auto itr = m_vAnimations.begin(); itr != m_vAnimations.end();)
	{
		if (*itr) {
			delete *itr;
			itr = m_vAnimations.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	m_vAnimations.clear();
}

void AnimationSet::AddAnimation(Animation* animation)
{
	m_vAnimations.push_back(animation);
}

int AnimationSet::getNumAnimations() const
{
	return m_vAnimations.size();
}

void AnimationSet::reset()
{
	for (auto itr : m_vAnimations)
	{
		itr->setCurrentKeyframe(1);
	}
}

void AnimationSet::update(const float delta_time)
{
	m_currTime = m_currTime + delta_time;

	for (auto itr : m_vAnimations)
	{
		itr->update(delta_time);
	}

	if (m_currTime > m_duration && !m_bLooping) {
		m_active = false;
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

	for (auto itr : m_vAnimations)
	{
		itr->update(delta_time);
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

bool AnimationSet::isLooping()
{
	return m_bLooping;
}

void AnimationSet::SetLooping(const bool looping)
{
	m_bLooping = looping;
}

};
