#include "Skeleton.h"
#include "Joint.h"
#include <stdio.h>
#include <utility>

#define C_STR(string, text)\
	(string + text).c_str()

namespace DE
{

Skeleton::Skeleton(const char* name)
	: m_vJoints(0)
	, m_vGlobalPose(0)
	, m_vWorldGlobalPose(0)
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
	m_vJoints.Resize(iNumJoints);
	m_vGlobalPose.Resize(iNumJoints);
	m_vWorldGlobalPose.Resize(iNumJoints);
	for (int i = 0; i < iNumJoints; ++i)
	{
		fscanf(pFile, "%s", &c);
		for (int j = 0; j < 16; ++j)
		{
			fscanf(pFile, "%f", &transform[j]);
		}
		fscanf(pFile, "%i", &parentIndex);
		m_vJoints.Add(Joint(transform, parentIndex));
		m_vGlobalPose.Add(new Matrix4);
		m_vWorldGlobalPose.Add(new Matrix4);
	}
	fclose(pFile);
}

Skeleton::~Skeleton()
{
	m_vJoints.Clear();
	m_vGlobalPose.Clear();
}

void Skeleton::Update(float deltaTime)
{
}

};