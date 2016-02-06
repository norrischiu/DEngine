#include "Animation.h"


Animation::Animation(const std::string node_name, const int frame = 0) :
	m_node_name(node_name), m_currKeyframe(frame)
{

}

Animation::~Animation()
{
}

void Animation::addAnimationMatrix(const Matrix4& matrix)
{
	m_animationMatrices.push_back(matrix);
}

void Animation::addAnimationMatrix(const std::vector<Matrix4>& matrices)
{
	m_animationMatrices.insert(m_animationMatrices.end(), matrices.begin(), matrices.end());
}

void Animation::setAnimationMatrices(const std::vector<Matrix4>& matrices)
{
	m_animationMatrices = matrices;
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
	if (frame >= 1 && frame <= 30) {
		m_currKeyframe = frame;
	}
}

const Matrix4& Animation::getCurrentMatrix() const
{
	return m_animationMatrices.at(getCurrentKeyframe() - 1);
}


void Animation::update(const float delta_time)
{
	int newFrame = ((int) (m_currKeyframe + (delta_time / (1.0f / 30.0f))));
	if (newFrame < 0) {
		newFrame = 30 - ((-newFrame) % 30);
	} else {
		newFrame = newFrame % 30;
	}

	setCurrentKeyframe(newFrame);
}