#include "AnimationSet.h"

namespace DE
{

AnimationSet::AnimationSet(const float duration, const unsigned int size, const float currTime, const bool active, const bool looping)
	: m_fCurrTime(currTime)
	, m_fDuration(duration)
	, m_bActive(active)
	, m_bLooping(looping)
	, m_vAnimations(size)
{

}

AnimationSet::~AnimationSet()
{
	m_vAnimations.Clear();
}

void AnimationSet::AddAnimation(Animation* animation)
{
	m_vAnimations.Add(animation);
}

void AnimationSet::reset()
{
	const unsigned int size = m_vAnimations.Size();
	for (int i = 0; i < size; ++i)
	{
		m_vAnimations[i]->setCurrentKeyframe(1);
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

		const unsigned int size = m_vAnimations.Size();
		for (int i = 0; i < size; ++i)
		{
			m_vAnimations[i]->update(delta_time);
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
