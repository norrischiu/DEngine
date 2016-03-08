#pragma once

#include <unordered_map>
#include <vector>
#include "Joint.h"
#include "Object\Component.h"

namespace DE
{

class Skeleton : public Component
{

public:

	static const int ComponentID = ComponentID::SKELETON;

	Skeleton(const char* name);
	~Skeleton();

	inline int GetJointsCount()
	{
		return m_vJoints.size();
	}

	inline Matrix4& GetGlobalPoseAt(int index)
	{
		return m_vGlobalPose[index];
	}

	// Inherited via Component
	virtual void Update(float deltaTime) override;

	std::vector<Joint>							m_vJoints;
	std::vector<Matrix4>						m_vGlobalPose;
};

};