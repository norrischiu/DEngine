#pragma once

#include <unordered_map>
#include <string>
#include "Math\simdmath.h"
#include "Math\SQT.h"
#include "Skeleton.h"

namespace DE
{

class Animation
{

public:
	Animation(const float animationFPS = 30.0f, const int frame = 1);
	~Animation();

	void AddPose(SQT sqt);

	int getCurrentKeyframe() const;
	int getNumKeyframes() const;
	void setCurrentKeyframe(const int frame);
	float getAnimationFPS();
	void setAnimationFPS(const float animationFPS);

	SQT GetCurrentPose(const float delta_time);

	void update(const float delta_time);

	//private:
	int m_currKeyframe;
	float m_animationFPS;
	std::vector<SQT> m_Poses;
};

};