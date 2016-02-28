#include "Animation.h"

Animation::Animation(const std::string node_name, const float animationFPS, const int frame) :
	m_node_name(node_name), m_animationFPS(animationFPS), m_currKeyframe(frame)
{

}

Animation::~Animation()
{
}

void Animation::AddPose(SQT sqt)
{
	m_Poses.push_back(sqt);
}

std::string Animation::getNodeName() const
{
	return m_node_name;
}

void Animation::setNodeName(std::string node_name)
{
	m_node_name = node_name;
}

int Animation::getCurrentKeyframe() const
{
	return m_currKeyframe;
}

int Animation::getNumKeyframes() const
{
	return m_Poses.size();
}

void Animation::setCurrentKeyframe(const int frame)
{
	if (frame >= 1 && frame <= m_Poses.size()) {
		m_currKeyframe = frame;
	}
}

SQT Animation::GetCurrentPose(const float delta_time)
{
	const float frameOffset = delta_time / (1.0f / m_animationFPS);
	const float interpolant = frameOffset - floor(frameOffset);

	if (interpolant > std::numeric_limits<float>::epsilon()) {
		const int numFrame = m_Poses.size();
		return SQT::LerpSQT(m_Poses[m_currKeyframe - 1], m_Poses[m_currKeyframe % numFrame], interpolant);
	}

	return m_Poses[m_currKeyframe - 1];
}

void Animation::update(const float delta_time)
{
	const int numFrame = m_Poses.size();
	const int frameOffset = ((int) (delta_time / (1.0f / m_animationFPS)));

	int newFrame = (m_currKeyframe + frameOffset) % numFrame;
	if (newFrame <= 0) {
		newFrame = numFrame - (-newFrame);
	}

	setCurrentKeyframe(newFrame);
}