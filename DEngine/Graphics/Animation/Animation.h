#pragma once

// Engine include
#include "Math\simdmath.h"
#include "Math\SQT.h"
#include "Skeleton.h"
#include "Utilities\MyArray.h"

namespace DE
{

/*
*	CLASS: Animation
*	Animation is the at the lowest level of the animation system. It
*	contains the exact transform in SQT form needed to move vertices
*	in a model as playing animation, it return the correct frame to
*	animation controller according to the game time
*/
class Animation
{

public:
	/********************************************************************************
	*	--- Constructor:
	*	Animation(const float, const float)
	*	This constructor will construct an animation class
	*
	*	--- Parameters:
	*	@ size: the initial size of all frames
	*	@ animationFPS: the animation frames per second in the animation definition
	*	file
	*	@ frame: the initial frame index
	********************************************************************************/
	Animation(size_t size = 0, const float animationFPS = 30.0f, const float frame = 1.0f);

	/********************************************************************************
	*	--- Destructor:
	*	~Animation()
	*	This destructor will free all memory used by the array of SQT
	********************************************************************************/
	~Animation();

	/********************************************************************************
	*	--- Function:
	*	AddPose(SQT)
	*	This function will add a SQT to the poses array
	*
	*	--- Parameters:
	*	@ sqt: a single SQT pose read from animation definition file
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddPose(SQT sqt);

	/********************************************************************************
	*	--- Function:
	*	setCurrentKeyframe(const float)
	*	This function will set the current key frame index, in float to allow
	*	interpolation between frames
	*
	*	--- Parameters:
	*	@ frame: the animation key frame index
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void setCurrentKeyframe(const float frame);

	/********************************************************************************
	*	--- Function:
	*	GetCurrentPose()
	*	This function will return the current pose in SQT format
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ SQT: the suitable pose in SQT format at this moment
	********************************************************************************/
	SQT GetCurrentPose();

	/********************************************************************************
	*	--- Virtual Function:
	*	update(const float)
	*	This function will update the current key frame index according to game time
	*
	*	--- Parameters:
	*	@ delta_time: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void update(const float delta_time);

private:

	float							m_currKeyframe;		// the current key frame index
	float							m_animationFPS;		// the animation frames per second defined in animation definition file
	MyArray<SQT>					m_Poses;
};

};