#include "Skeleton.h"
#include "Joint.h"
#include <stdio.h>
#include <utility>

#define C_STR(string, text)\
		(string + text).c_str()

namespace DE
{

Skeleton::Skeleton(const char* name)
{
	m_ID = ComponentID;

	std::string sFileName(name);
	sFileName = "../Assets/" + sFileName;
	FILE* pFile = fopen(C_STR(sFileName, "_skeleton.bufa"), "r");
	char c[256];
	int iNumJoints, parentIndex;
	float transform[16];
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%i", &iNumJoints);
	m_vGlobalPose.resize(iNumJoints, Matrix4::Identity);
	for (int i = 0; i < iNumJoints; ++i)
	{
		fscanf(pFile, "%s", &c);
		for (int j = 0; j < 16; ++j)
		{
			fscanf(pFile, "%f", &transform[j]);
		}
		fscanf(pFile, "%i", &parentIndex);
		m_vJoints.push_back(new Joint(transform, parentIndex));
	}
	fclose(pFile);
}

Skeleton::~Skeleton()
{
}
/*
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
*/
void Skeleton::Update(float deltaTime)
{
}

};