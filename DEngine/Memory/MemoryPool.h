// C++ include
#include <assert.h>
#include <stdint.h>
#include <memory>

const unsigned int MEMORY_ALIGNMENT = 16;	// memory alignment requirement

/*
*	class: MemoryPool
*	MemoryPool keep track of the memory pool and block
*	status, and is responsible to return appropriate
*	block of memory when Handle or other system requests
*/
class MemoryPool
{
public:

	/********************************************************************************
	*	--- Static Function:
	*	Construct(size_t, unsigned int, void*&)
	*	This function will construct a memory pool with alignment at the beginning
	*
	*	--- Parameters:
	*	@ size: size of the each memory block
	*	@ num: number of memory block
	*	@ heapStart: the address of the start of the heap after the last memory pool
	*
	*	--- Return:
	*	@ MemoryPool*: pointer to the aligned address of start of memory pool
	********************************************************************************/
	static MemoryPool* Construct(size_t size, unsigned int num, void* &heapStart)
	{
		assert(size % MEMORY_ALIGNMENT == 0); // Make sure the block size does not need alignment

		MemoryPool* ptr = (MemoryPool*) heapStart;
		ptr->m_iBlockSize = size;
		ptr->m_iFreeBlockNum = num;
		ptr->m_iFreeBlockIndex = 0;

		heapStart = (void*) ((uint64_t) heapStart + sizeof(unsigned int) * 4);

		for (unsigned int i = 0; i < num; ++i)
		{
			ptr->m_pFreeList[i] = i;
		}

		heapStart = (void*)((uint64_t) heapStart + (size * num) + (sizeof(unsigned int) * num));
		return ptr;
	}

	unsigned int							m_iBlockSize;		// block size of this memory pool
	unsigned int							m_iFreeBlockNum;		// number of free memory block
	unsigned int							m_iFreeBlockIndex;		// the index of next free memory block in the free list
	unsigned int							m_pFreeList[63356];		// free memory block indices 

private:

	/********************************************************************************
	*	--- Constructor:
	*	MemoryPool()
	*	This constructor will do nothing
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	MemoryPool(){}
};
