#include "Skeleton.h"
#include <utility>

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::addJoint(const std::string name, const Joint& joint)
{
	std::pair <std::string, Joint> t_joint = std::make_pair(name, joint);
	m_joints.insert(t_joint);
}

void Skeleton::removeJoint(const std::string name)
{
	m_joints.erase(name);
}

Joint* Skeleton::findJoint(const std::string name)
{
	auto t = m_joints.find(name);

	if (t == m_joints.end()) {
		return nullptr;
	}
	else {
		return &(t->second);
	}

	return nullptr;
}

int Skeleton::getNumJoints() const
{
	return m_joints.size();
}