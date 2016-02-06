#include "Animation.h"


Animation::Animation(const float length) :
	m_length(length)
{

}

Animation::~Animation()
{
}

void Animation::addAnimationMatrix(const float time, const Matrix4& matrix)
{
	m_times.push_back(time);
	m_animationMatrixs.push_back(matrix);
}

void Animation::animate(Skeleton* skleton, const float time)
{

}