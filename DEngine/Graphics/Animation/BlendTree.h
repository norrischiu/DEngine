#ifndef BLEND_TREE_H_
#define BLEND_TREE_H_

// Engine include
#include "Memory\Handle.h"

// C++ include
#include <vector>

namespace DE
{

struct BlendTree
{
	enum eBlendType
	{
		ONE_D, TWO_D
	};

	CUSTOM_MEMORY_DEFINE();

	std::vector<char*>  m_vClipnames;
	std::vector<float>  m_vWeightings;
	float m_fBlendFactor;
	eBlendType m_eBlendType;
};

}

#endif //!BLEND_TREE_H_