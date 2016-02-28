#pragma once

#include <unordered_map>
#include <string>
#include "Math\simdmath.h"
#include "Math\SQT.h"
#include "Skeleton.h"

class Animation
{

public:
	Animation(const std::string node_name, const float animationFPS = 30.0f, const int frame = 0);
	~Animation();

	void AddPose(SQT sqt);
	
	std::string getNodeName() const;
	void setNodeName(const std::string node_name);

	int getCurrentKeyframe() const;
	int getNumKeyframes() const;
	void setCurrentKeyframe(const int frame);

	SQT GetCurrentPose(const float delta_time);

	void update(const float delta_time);

//private:
	std::string m_node_name;
	int m_currKeyframe;
	float m_animationFPS;
	std::vector<SQT> m_Poses;
};

