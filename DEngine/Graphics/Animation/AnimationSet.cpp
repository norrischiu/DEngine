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

		if (m_fCurrTime > m_fDuration && !m_bLooping) 
		{
			m_bActive = false;
			m_fCurrTime = 0.0f;
			return;
		}

		for (auto itr : m_vAnimations)
		{
			itr->update(delta_time);
		}
	}
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
