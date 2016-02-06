#pragma once

#include <unordered_map>
#include "SkeletonNode.h"

class Skeleton
{
public:
	Skeleton(const int num);
	~Skeleton();

	void addSkeletonNode(const std::string name, const SkeletonNode& skeletonNode);
	void removeSkeletonNode(const std::string name);
	SkeletonNode* findSkeletonNode(const std::string name);
	void updateSkeletonNode(SkeletonNode* skeletonNode, const Matrix4& matrix);
	void updateSkeletonNode(const std::string name, const Matrix4& matrix);

	std::unordered_map<std::string, SkeletonNode>* getSkeletonTree();
	void setSkeletonTree(const std::unordered_map<std::string, SkeletonNode>& skeletonTree);

	int getNumSkeletonNodes() const;
	void setNumSkeletonNodes(const int num);

private:
	std::unordered_map<std::string, SkeletonNode> m_skeletonTree;
	int m_numSkeletonNodes;
};
