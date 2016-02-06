#pragma once

#include <unordered_map>
#include "Joint.h"

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	void addJoint(const std::string name, const Joint& joint);
	void removeJoint(const std::string name);
	Joint* findJoint(const std::string name);

	int getNumJoints() const;

private:
	std::unordered_map<std::string, Joint> m_joints;
};
