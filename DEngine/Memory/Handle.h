// Handle.h: the struct for a handle (array index) to reference memory location
#ifndef HANDLE_H_
#define HANDLE_H_

#include <stdint.h>
#include "MemoryManager.h"

#define CUSTOM_MEMORY_DEFINE()\
	void* operator new(size_t size, Handle hle)\
	{ return MemoryManager::GetInstance()->GetMemoryAddressFromHandle(hle); }\
	void operator delete(void* ptr, Handle hle)\
	{ return MemoryManager::GetInstance()->Free(hle); }

struct Handle
{
	Handle() {};

	// Constructor
	Handle(uint32_t poolIndex, uint32_t blockIndex, uint32_t counter = 0)
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

	inline void* Raw()
	{
		return MemoryManager::GetInstance()->GetMemoryAddressFromHandle(*this);
	}

	inline void Free()
	{
		MemoryManager::GetInstance()->Free(*this);
	}
};

#endif