#pragma once

#include <unordered_map>
#include <vector>
#include "SkeletonNode.h"
#include "Joint.h"
#include "Object\Component.h"

class Skeleton : public Component
{

public:

	static const int ComponentID = ComponentID::SKELETON;

	Skeleton(const char* name);
	~Skeleton();

	//void addSkeletonNode(const std::string name, const SkeletonNode& skeletonNode);
	//void removeSkeletonNode(const std::string name);
	//SkeletonNode* findSkeletonNode(const std::string name);
	//void updateSkeletonNode(SkeletonNode* skeletonNode, const Matrix4& matrix);
	//void updateSkeletonNode(const std::string name, const Matrix4& matrix);

	//std::unordered_map<std::string, SkeletonNode>* getSkeletonTree();
	//void setSkeletonTree(const std::unordered_map<std::string, SkeletonNode>& skeletonTree);

	//int getNumSkeletonNodes() const;
	//void setNumSkeletonNodes(const int num);

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

	std::vector<Joint*>							m_vJoints;
	std::vector<Matrix4>						m_vGlobalPose;
};
