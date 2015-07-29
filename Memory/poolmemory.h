#pragma once

#include <assert.h>
#include <memory.h>

/** Pool memory pool block and allocator (singleton) class */

// 16 bytes aligned memory block
template<size_t blockSize>
class PoolMemoryBlock
{
public:
	// Include the next pointer inside the memory block
	union
	{
		// The actual memory
		char							_memory[blockSize];

		// Pointer to next free memory block
		PoolMemoryBlock<blockSize>*		_next;
	};

	// Default constructor
	PoolMemoryBlock()
		:_next(NULL)
	{
		// Make sure the block size does not need alignment
		assert(blockSize % 16 == 0);	
	};

	// Overload new operator
	void* operator new[](size_t size)
	{
		// 16 bytes aligned memory allocation
		size_t alignment = 16;
		size_t expandedSize = size + alignment;
		uintptr_t rawAddress = reinterpret_cast<uintptr_t>(malloc(expandedSize));

		// Calculate the aligned address by a 15 bytes mask
		size_t mask = alignment - 1;
		uintptr_t misalignment = rawAddress & mask;
		ptrdiff_t adjustment = alignment - misalignment; // a negative int
		uintptr_t alignedAddress = rawAddress + adjustment;
		
		// Store the adjustment in the previous memory block's last byte (guaranteed to be empty)
		uint8_t* pAlignedAddress = reinterpret_cast<uint8_t*>(alignedAddress);
		pAlignedAddress[-1] = static_cast<uint8_t>(adjustment);

		return static_cast<void*>(pAlignedAddress);

		// MSVC++ function
		// return _aligned_malloc(size, 16);
	}

	// Overload delete operator
	void operator delete[](void* ptr)
	{
		// Acquire the raw address of (size + alignment)
		uint8_t* pAlignedMemory = reinterpret_cast<uint8_t*>(ptr);
		ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMemory[-1]);
		uintptr_t pAlignedAddress = reinterpret_cast<uintptr_t>(ptr);
		uintptr_t rawAddress = pAlignedAddress - adjustment;
		void* pRawAddress = reinterpret_cast<void*>(rawAddress);

		free(pRawAddress);

		// MSVC++ function
		// _aligned_free(ptr);
	}
};

// Singleton memory allocator
template<size_t blockSize, unsigned int numBlock>
class PoolMemoryManager
{
public:
	// Return the only instance
	static PoolMemoryManager* getInstance()
	{
		if (!m_pInstance)
			m_pInstance = new PoolMemoryManager();
		return m_pInstance;
	};

	void Construct();
	void Destruct();

	// Allocate a memory block of size from the pool, return the pointer to the block
	void* Allocate(size_t size);

	// Free the memory block back to the pool
	void Free(void* ptr);
private:
	// Default constructor
	PoolMemoryManager()
		:m_pFreeList(NULL), m_pPool(NULL){};

	// Singleton instance
	static PoolMemoryManager*				m_pInstance;

	// Free memory pool
	PoolMemoryBlock<blockSize>*				m_pFreeList;

	// All memory blocks' pool
	PoolMemoryBlock<blockSize>*				m_pPool;
};

// Implementation
template<size_t blockSize, unsigned int numBlock>
PoolMemoryManager<blockSize, numBlock>* PoolMemoryManager<blockSize, numBlock>::m_pInstance = 0;

template<size_t blockSize, unsigned int numBlock>
void PoolMemoryManager<blockSize, numBlock>::Construct()
{
	m_pPool = new PoolMemoryBlock<blockSize>[numBlock];
	m_pFreeList = m_pPool;

	// Configure free list pointer
	PoolMemoryBlock<blockSize>* ptr = m_pFreeList;
	for (unsigned int i = 1; i < numBlock; ++i)
	{
		ptr->_next = &m_pPool[i];
		ptr = ptr->_next;
	}
}

template<size_t blockSize, unsigned int numBlock>
void PoolMemoryManager<blockSize, numBlock>::Destruct()
{
	delete[] m_pPool;
	
	// Set pointers to NULL
	m_pFreeList = NULL;
	m_pPool = NULL;
}

template<size_t blockSize, unsigned int numBlock>
void* PoolMemoryManager<blockSize, numBlock>::Allocate(size_t size)
{
	assert(size <= blockSize);
	if (!m_pFreeList)
		return NULL;

	PoolMemoryBlock<blockSize>* requiredBlock = m_pFreeList;
	m_pFreeList = m_pFreeList->_next;
	
	return requiredBlock->_memory;
}

template<size_t blockSize, unsigned int numBlock>
void PoolMemoryManager<blockSize, numBlock>::Free(void* ptr)
{
	if (m_pFreeList)
	{
		PoolMemoryBlock<blockSize>* temp = m_pFreeList;
		m_pFreeList = reinterpret_cast<PoolMemoryBlock<blockSize>*>(ptr);
		m_pFreeList->_next = temp;
	}
	// Special case: free pool is empty already
	else
	{
		m_pFreeList = reinterpret_cast<PoolMemoryBlock<blockSize>*>(ptr);
		m_pFreeList->_next = NULL;
	}
}
