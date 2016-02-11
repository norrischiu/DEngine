#pragma once

#include <unordered_map>
#include <string>
#include "Math\simdmath.h"
#include "Math\SQT.h"
#include "Skeleton.h"

class Animation
{

public:
	Animation(const std::string node_name, const int frame = 1);
	~Animation();

	void addAnimationMatrix(const Matrix4& matrix);
	void addAnimationMatrix(const std::vector<Matrix4>& matrices);

	void AddPose(SQT sqt);

	std::vector<Matrix4>& getAnimationMatrices();
	void setAnimationMatrices(const std::vector<Matrix4>& matrices);
	
	std::string getNodeName() const;
	void setNodeName(const std::string node_name);

	int getCurrentKeyframe() const;
	void setCurrentKeyframe(const int frame);

	const Matrix4& getCurrentMatrix() const;

	SQT GetCurrentPose();

	void update(const float delta_time);

private:
	std::vector<Matrix4> m_animationMatrices;
	std::string m_node_name;
	int m_currKeyframe;

	std::vector<SQT>					m_Poses;
	int									m_iNumFrame;
};

