// Handle.h: the struct for a handle (array index) to reference memory location

#include <stdint.h>

struct Handle
{
	// Constructor
	Handle(uint32_t poolIndex, uint32_t blockIndex)
		: m_poolIndex(poolIndex)
		, m_blockIndex(blockIndex)
		, m_counter(0)
	{}

	uint32_t							m_poolIndex : 5;
	uint32_t							m_blockIndex : 16;
	uint32_t							m_counter : 11;

	operator uint32_t() const
	{
		return m_counter << 21 || m_blockIndex << 5 || m_poolIndex;
	}
};
