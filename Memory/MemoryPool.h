// MemoryPool.h: 

#include <assert.h>
#include <stdint.h>
#include <memory>

const unsigned int MEMORY_ALIGNMENT = 16;

class MemoryPool
{
public:

	unsigned int							m_iBlockSize;
	unsigned int							m_iFreeBlockNum;
	unsigned int							m_iFreeBlockIndex;
	unsigned int							m_pFreeList[63356]; // indices

	// Construct a memory pool with alignment at the beginning
	static MemoryPool* Construct(size_t size, unsigned int num, void* &heapStart)
	{
		assert(size % MEMORY_ALIGNMENT == 0); // Make sure the block size does not need alignment

		MemoryPool* ptr = (MemoryPool*) heapStart;
		ptr->m_iBlockSize = size;
		ptr->m_iFreeBlockNum = num;
		ptr->m_iFreeBlockIndex = 0;

		heapStart = (void*) ((uint32_t) heapStart + sizeof(unsigned int) * 3);

		for (unsigned int i = 0; i < num; ++i)
		{
			ptr->m_pFreeList[i] = i;
		}

		heapStart = (void*)((uint32_t) heapStart + (size * num));
		return ptr;
	}

private:

	// Constructor
	MemoryPool(){}
};
