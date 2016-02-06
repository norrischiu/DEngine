#pragma once

#include <unordered_map>
#include "Joint.h"

class SkeletonNode
{
public:
	SkeletonNode(const Joint& joint);
	~SkeletonNode();

	void addChildNode(const Joint& joint);
	void addChildren(const std::vector<Joint>& joints);

	Joint* getJoint();
	void setJoint(const Joint& joint);
	std::vector<SkeletonNode>* getChildren();
	int getNumChildren() const;

private:
	Joint m_joint;
	std::vector<SkeletonNode> m_children;
};
