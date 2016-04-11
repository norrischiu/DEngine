#include "AnimationSet.h"

namespace DE
{

AnimationSet::AnimationSet(const float currTime, const float duration, const bool active, const bool looping) :
	m_fCurrTime(currTime), m_fDuration(duration), m_bActive(active), m_bLooping(looping)
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
		m_fCurrTime = 0.0f;
	}
}

void AnimationSet::update(const float delta_time)
{
	if (m_bActive)
	{
		m_fCurrTime = m_fCurrTime + delta_time;

		for (auto itr : m_vAnimations)
		{
			itr->update(delta_time);
		}

		if (m_fCurrTime > m_fDuration && !m_bLooping) {
			m_bActive = false;
			m_fCurrTime = 0.0f;
		}
	}
}

float AnimationSet::getCurrTime() const
{
	return m_fCurrTime;
}

void AnimationSet::setCurrTime(const float currTime)
{
	const int delta_time = currTime - m_fCurrTime;
	m_fCurrTime = currTime;

	for (auto itr : m_vAnimations)
	{
		itr->update(delta_time);
	}
}

float AnimationSet::getDuration() const
{
	return m_fDuration;
}

void AnimationSet::setDuration(const float duration)
{
	m_fDuration = duration;
}

bool AnimationSet::isActive() const
{
	return m_bActive;
}

void AnimationSet::setActive(const bool active)
{
	m_bActive = active;
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
