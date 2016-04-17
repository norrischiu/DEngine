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
	Animation(const float animationFPS = 30.0f, const float frame = 1.0f);
	~Animation();

	void AddPose(SQT sqt);

	int getCurrentKeyframe() const;
	int getNumKeyframes() const;
	void setCurrentKeyframe(const float frame);
	float getAnimationFPS();
	void setAnimationFPS(const float animationFPS);

	SQT GetCurrentPose();

	void update(const float delta_time);

	//private:
	float m_currKeyframe;
	float m_animationFPS;
	std::vector<SQT> m_Poses;
};

};