#include "Skeleton.h"
#include <utility>

Skeleton::Skeleton(const int num) : m_numSkeletonNodes(num)
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::addSkeletonNode(const std::string name, const SkeletonNode& skeletonNode)
{
	std::pair <std::string, SkeletonNode> node = std::make_pair(name, skeletonNode);
	m_skeletonTree.insert(node);
}

void Skeleton::removeSkeletonNode(const std::string name)
{
	m_skeletonTree.erase(name);
}

SkeletonNode* Skeleton::findSkeletonNode(const std::string name)
{
	auto t = m_skeletonTree.find(name);

	if (t == m_skeletonTree.end()) {
		return nullptr;
	}
	else {
		return &(t->second);
	}

	return nullptr;
}

void Skeleton::updateSkeletonNode(SkeletonNode* skeletonNode, const Matrix4& matrix)
{
	skeletonNode->getJoint()->setCurrMatrix(matrix);

	for (SkeletonNode node : *(skeletonNode->getChildren())) {
		updateSkeletonNode(&node, matrix);
	}
}

void Skeleton::updateSkeletonNode(const std::string name, const Matrix4& matrix)
{
	//Get joints in the sub tree of by the name of the root joint of the sub tree
	//Apply the matrix to each joint

	SkeletonNode* skeletonNode = findSkeletonNode(name);

	if (skeletonNode) {
		updateSkeletonNode(skeletonNode, matrix);
	}
}

std::unordered_map<std::string, SkeletonNode>* Skeleton::getSkeletonTree()
{
	return &m_skeletonTree;
}

void Skeleton::setSkeletonTree(const std::unordered_map<std::string, SkeletonNode>& skeletonTree)
{
	m_skeletonTree = skeletonTree;
}

int Skeleton::getNumSkeletonNodes() const
{
	return m_numSkeletonNodes;
}

void Skeleton::setNumSkeletonNodes(const int num)
{
	m_numSkeletonNodes = num;
}