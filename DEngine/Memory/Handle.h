// Handle.h: the struct for a handle (array index) to reference memory location
#ifndef HANDLE_H_
#define HANDLE_H_

// Engine include
#include "MemoryManager.h"

// C++ include
#include <stdint.h>

#define CUSTOM_MEMORY_DEFINE()\
	void* operator new(size_t size, Handle hle)\
	{ return MemoryManager::GetInstance()->GetMemoryAddressFromHandle(hle); }\
	void operator delete(void* ptr, Handle hle)\
	{ return MemoryManager::GetInstance()->Free(hle); }

namespace DE
{

/*
*	STRUCT: Handle
*	Handle replaces dynamically allocated memory by storing
*	the pool index and block index, so by getting raw pointer
*	of the Handle user can get the memory of the block
*/
struct Handle
{

	/********************************************************************************
	*	--- Constructor:
	*	Handle()
	*	This constructor will construct an invalid Handle with empty content, and
	*	counter at 0, user needs to call Set() in order for valid use
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	Handle() 
		: m_counter(0)
	{};

	/********************************************************************************
	*	--- Constructor:
	*	Handle(size_t)
	*	This constructor will construct an Handle referring to the memory of or
	*	larger than the given size, typically use with sizeof(class)
	*
	*	--- Parameters:
	*	@ size: size of the memory needed
	********************************************************************************/
	Handle(size_t size)
		: m_counter(1)
	{
		*this = MemoryManager::GetInstance()->Allocate(size);
	};

	/********************************************************************************
	*	--- Constructor:
	*	Handle(uint32_t, uint32_t, uint32_t)
	*	This constructor will construct an Handle with given index
	*
	*	--- Parameters:
	*	@ poolIndex: memory pool index
	*	@ blockIndex: memory block index
	*	@ counter: number of reference referring to this piece of memory
	********************************************************************************/
	Handle(uint32_t poolIndex, uint32_t blockIndex, uint32_t counter = 0)
		: m_poolIndex(poolIndex)
		, m_blockIndex(blockIndex)
		, m_counter(1)
	{}

	/********************************************************************************
	*	--- Function:
	*	Set(size_t)
	*	This function will allocate memory of given size to this Handle, should only
	*	be called from an invalid Handle (i.e. constructed with empty constructor)
	*
	*	--- Parameters:
	*	@ size: size of the memory needed
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Set(size_t size)
	{
		assert(m_counter == 0);
		m_counter++;
		*this = MemoryManager::GetInstance()->Allocate(size);
	}

	/********************************************************************************
	*	--- Function:
	*	uint32_t()
	*	This function will return an integer with concatenated information of the
	*	Handle, can be used for display or compare
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ uint32_t: concatenated integer
	********************************************************************************/
	operator uint32_t() const
	{
		return m_counter << 21 || m_blockIndex << 5 || m_poolIndex;
	}

	/********************************************************************************
	*	--- Function:
	*	Raw()
	*	This function will return the memory address that this Handle is referring to
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void*: memory address that this Handle referring to
	********************************************************************************/
	inline void* Raw()
	{
		return MemoryManager::GetInstance()->GetMemoryAddressFromHandle(*this);
	}

	/********************************************************************************
	*	--- Function:
	*	Free()
	*	This function will release the block back to the memory pool if this Handle
	*	is only having one reference (the caller), and decrement counter if this Handle
	*	is having more than one reference
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	inline void Free()
	{
		if (m_counter == 1)
		{
			MemoryManager::GetInstance()->Free(*this);
		}
		m_counter--;
	}

	uint32_t							m_poolIndex : 5;		// pool index occupying 5 bits
	uint32_t							m_blockIndex : 16;		// pool index occupying 16 bits
	uint32_t							m_counter : 11;		// counter occupying 11 bits
};

};
#endif