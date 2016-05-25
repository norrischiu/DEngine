#ifndef BLEND_TREE_H_
#define BLEND_TREE_H_

// Engine include
#include "Memory\Handle.h"

// C++ include
#include <vector>

namespace DE
{

/*
*	STRUCT: BlendTree
*	Blend tree contains names of numbers of animation clip and their respective
*	weighting, it is used by animation controller to calculate a blended pose
*/
struct BlendTree
{
	/*
	*	ENUM: eBlendType
	*	Blend type in this blend tree
	*/
	enum eBlendType
	{
		ONE_D,		// 1D/linear blending
		TWO_D		// 2D blending
	};

	CUSTOM_MEMORY_DEFINE();

	std::vector<char*>			m_vClipnames;		// array of names of animation clip
	std::vector<float>			m_vWeightings;		// array of weighting according to the order in names array
	float						m_fBlendFactor;		// the current blend factor between clips
	eBlendType					m_eBlendType;		// the blend type
};

}

#endif //!BLEND_TREE_H_