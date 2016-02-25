#include "Animation.h"

Animation::Animation(const std::string node_name, const int frame) :
	m_node_name(node_name), m_currKeyframe(frame)
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

void Animation::setCurrentKeyframe(const int frame)
{
	if (frame >= 1 && frame <= m_Poses.size()) {
		m_currKeyframe = frame;
	}
}

SQT Animation::GetCurrentPose()
{
	return m_Poses[m_currKeyframe - 1];
}

void Animation::update(const float delta_time)
{
	int newFrame = (m_currKeyframe + ((int) (delta_time / (1.0f / 30.0f)))) % m_Poses.size();
	if (newFrame <= 0) {
		newFrame = m_Poses.size() - (-newFrame);
	}

	setCurrentKeyframe(newFrame);
}