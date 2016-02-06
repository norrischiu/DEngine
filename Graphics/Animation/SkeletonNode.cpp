#include "SkeletonNode.h"
#include <utility>

SkeletonNode::SkeletonNode(const Joint& joint) :
	m_joint(joint)
{

}

void SkeletonNode::addChildNode(const Joint& joint)
{
	m_children.push_back(joint);
}

void SkeletonNode::addChildren(const std::vector<Joint>& joints)
{
	//Merge two vectors
	m_children.insert(m_children.end(), joints.begin(), joints.end());
}

std::vector<SkeletonNode>* SkeletonNode::getChildren()
{
	return &m_children;
}

Joint* SkeletonNode::getJoint()
{
	return &m_joint;
}

int SkeletonNode::getNumChildren() const
{
	return m_children.size();
}

SkeletonNode::~SkeletonNode()
{

}