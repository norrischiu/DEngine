#pragma once

// Engine include
#include "Joint.h"
#include "Object\Component.h"

// C++ include
#include <vector>

namespace DE
{

struct Skeleton : public Component
{
	/********************************************************************************
	*	--- Constructor:
	*	Skeleton(const char*)
	*	This constructor will construct a skeleton by reading a skeleton 
	*	definition file
	*
	*	--- Parameters:
	*	@ filename: the file name of the skeleton definition file in Assets folder
	********************************************************************************/
	Skeleton(const char* filename);

	/********************************************************************************
	*	--- Destructor:
	*	~Skeleton()
	*	This destructor will free all memory used by joints and poses
	********************************************************************************/
	~Skeleton();

	/********************************************************************************
	*	--- Function:
	*	GetJointsCount()
	*	This function return the number of joints in this skeleton
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ const int: number of joints in this skeleton
	********************************************************************************/
	inline const int GetJointsCount()
	{
		return m_vJoints.size();
	}

	/********************************************************************************
	*	--- Virtual Function:
	*	Update(float)
	*	This function is inherited via Component
	*	This function does nothing, but is required to override as a component
	*
	*	--- Parameters:
	*	@ deltaTime: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void Update(float deltaTime) override;

	static const int							ComponentID = ComponentID::SKELETON;	// Component ID
	std::vector<Joint>							m_vJoints;			// array of joints
	std::vector<Matrix4*>						m_vGlobalPose;		// Pose in model space
	std::vector<Matrix4*>						m_vWorldGlobalPose;	// Pose in world space
};

};