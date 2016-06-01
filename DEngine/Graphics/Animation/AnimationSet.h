#pragma once

#include "Animation.h"
#include "Utilities\MyArray.h"

namespace DE
{

/*
*	CLASS: AnimationSet
*	Animation set is a analog to a animation clip, it contains array of animations.
*	It also controls which frame of animation to be returned upon request.
*/
class AnimationSet
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	AnimationSet(const float, const float, const bool, const bool);
	*	This constructor will construct a empty animation set for later insertion of
	*	animations
	*
	*	--- Parameters:
	*	@ duration: the duration of the animation clip
	*	@ size: the initial size of all joints/animation struct
	*	@ currTime: the initial starting point of the animation from the first frame
	*	@ active: True if the animation set is initialized as active; False if
	*	the animation set is initialized as NOT active
	*	@ looping: True if the animation set represents a looping animation; False
	*	if this animation will not loop
	********************************************************************************/
	AnimationSet(const float duration, const unsigned int size, const float currTime = 0.0f, const bool active = false, const bool looping = false);

	/********************************************************************************
	*	--- Destructor:
	*	~AnimationSet()
	*	This destructor will free all memory used by animations array
	********************************************************************************/
	~AnimationSet();

	/********************************************************************************
	*	--- Function:
	*	AddAnimation(Animation*)
	*	This function will add a new animation to the array
	*
	*	--- Parameters:
	*	@ animation: an animation struct
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddAnimation(Animation* animation);

	/********************************************************************************
	*	--- Function:
	*	isActive()
	*	This function will check if this animation set is currently active
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ bool: True if this animation sets is active; False if this is NOT active
	********************************************************************************/
	bool isActive() const;

	/********************************************************************************
	*	--- Function:
	*	setActive(const bool)
	*	This function will set this animation set as active or NOT active
	*
	*	--- Parameters:
	*	@ active: True if intends to set animation set as active; False if intends
	*	to set animation set as NOT active
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void setActive(const bool active);

	/********************************************************************************
	*	--- Function:
	*	isLooping()
	*	This function will check this animation set consists of a looping animation
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ bool: True if this animation set represents a looping animation; False if 
	*	none is active
	********************************************************************************/
	bool isLooping();

	/********************************************************************************
	*	--- Function:
	*	SetLooping(const bool)
	*	This function will check if any of the animation set in a state is
	*	currently active
	*
	*	--- Parameters:
	*	@ looping: True if intends to set this animation set as looping; False if
	*	intends to set this animation as not looping and will
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetLooping(const bool looping);

	/********************************************************************************
	*	--- Function:
	*	reset()
	*	This function will reset the animation timeline by setting the current time
	*	to zero
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void reset();

	/********************************************************************************
	*	--- Function:
	*	Update(float)
	*	This function will update the current time in the animation timeline, and
	*	end the animation if it passed the duration
	*
	*	--- Parameters:
	*	@ delta_time: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void update(const float delta_time);

	MyArray<Animation*>		m_vAnimations;		// the array of animation struct

private:

	float					m_fCurrTime;	// the current time at the timeline of the animation set
	float					m_fDuration;	// the duration to finish playing this animation set
	bool					m_bActive;		// flag on whether this animation set is active
	bool					m_bLooping;		// flag on whether this animation set loops
};

};