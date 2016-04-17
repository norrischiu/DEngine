#include "Animation.h"

namespace DE
{

Animation::Animation(const float animationFPS, const float frame) :
	m_animationFPS(animationFPS), m_currKeyframe(frame)
{

}

Animation::~Animation()
{

}

void Animation::AddPose(SQT sqt)
{
	m_Poses.push_back(sqt);
}

int Animation::getCurrentKeyframe() const
{
	return m_currKeyframe;
}

int Animation::getNumKeyframes() const
{
	return m_Poses.size();
}

void Animation::setCurrentKeyframe(const float frame)
{
	if (frame >= 1.0f && frame <= (float)(m_Poses.size() + 1)) {
		m_currKeyframe = frame;
	}
}

float Animation::getAnimationFPS()
{
	return m_animationFPS;
}

void Animation::setAnimationFPS(const float animationFPS)
{
	m_animationFPS = animationFPS;
}

SQT Animation::GetCurrentPose()
{
	const float interpolant = m_currKeyframe - std::floor(m_currKeyframe);

	if (interpolant > std::numeric_limits<float>::epsilon()) {
		const int numFrame = m_Poses.size();
		const int lowerFrame = ((int)std::fmod(std::floor(m_currKeyframe - 1.0f), numFrame));
		const int upperFrame = ((int)std::fmod(std::ceil(m_currKeyframe - 1.0f), numFrame));
		return SQT::LerpSQT(m_Poses[lowerFrame], m_Poses[upperFrame], interpolant);
	}

	return m_Poses[m_currKeyframe - 1];
}

void Animation::update(const float delta_time)
{
	const int numFrame = m_Poses.size();
	const float frameOffset = delta_time / (1.0f / m_animationFPS);

	float newFrame = std::fmod(m_currKeyframe + frameOffset, numFrame);
	if (newFrame <= 1.0f) {
		newFrame = numFrame - (-newFrame);
	}

	setCurrentKeyframe(newFrame);
}

};