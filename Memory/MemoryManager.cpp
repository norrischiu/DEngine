// MemoryManager.cpp

#include "MemoryManager.h"

MemoryManager* MemoryManager::m_pInstance;

void MemoryManager::Construct()
{
	unsigned int heapSize = 0;

	for (unsigned int i = 0; i < MEMORY_POOL_NUM; ++i)
	{
		heapSize += MEMORY_POOL_CONFIG[i][0] * MEMORY_POOL_CONFIG[i][1] + sizeof(unsigned int) * 3;
	}
	m_pRawHeapStart = malloc(heapSize);
	void* heapStart = alignedAddress(m_pRawHeapStart); // align the start of memory pool

	for (unsigned int i = 0; i < MEMORY_POOL_NUM; ++i)
	{
		m_pPool[i] = MemoryPool::Construct(MEMORY_POOL_CONFIG[i][0], MEMORY_POOL_CONFIG[i][1], heapStart);
	}
}

void MemoryManager::Defragment()
{
}

void MemoryManager::Destruct()
{
	free(m_pRawHeapStart);
	*m_pPool = nullptr;
}

Handle MemoryManager::Allocate(size_t size)
{
	for (unsigned int i = 0; i < MEMORY_POOL_NUM; ++i) 
	{
		if (size <= MEMORY_POOL_CONFIG[i][0])
		{
			if (!m_pPool[i]->m_pFreeList)
			{
				assert(false || "no block left");
			}

			m_pPool[i]->m_iFreeBlockNum--;
			return Handle(i, m_pPool[i]->m_iFreeBlockIndex++);
		}
	}

	assert(false || "no block fits");
}

void MemoryManager::Free(Handle hle)
{
	unsigned int index = m_pPool[hle.m_poolIndex]->m_iFreeBlockNum++;
}

// Return a aligned address according to the alignment
void* MemoryManager::alignedAddress(void* ptr)
{
	unsigned int adjustment = (unsigned int) ptr % MEMORY_ALIGNMENT;
	if (adjustment)
	{
		//unsigned int* intptr = (unsigned int*) ptr;
		//intptr[0] = adjustment;
		return (void *)((unsigned int) ptr + MEMORY_ALIGNMENT - adjustment);
	}
	else
	{
		return ptr;
	}
}
