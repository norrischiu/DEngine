// MemoryManager.cpp

#include "MemoryManager.h"
#include "Handle.h"

namespace DE
{

MemoryManager* MemoryManager::m_pInstance;

void MemoryManager::ConstructDefaultPool()
{
	unsigned int heapSize = 0;

	for (unsigned int i = 0; i < MEMORY_POOL_NUM; ++i)
	{
		heapSize += MEMORY_POOL_CONFIG[i][0] * MEMORY_POOL_CONFIG[i][1] + sizeof(unsigned int) * 4 + sizeof(unsigned int) * MEMORY_POOL_CONFIG[i][1];
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
			if (m_pPool[i]->m_iFreeBlockNum == 0)
			{
				assert(false && "no block left");
			}
			int index = m_pPool[i]->m_pFreeList[m_pPool[i]->m_iFreeBlockIndex];
			m_pPool[i]->m_iFreeBlockNum--;
			m_pPool[i]->m_iFreeBlockIndex++;
			return Handle(i, index);
		}
	}

	assert(false && "no block fits");
}

void MemoryManager::Free(Handle hle)
{
	memset(hle.Raw(), 0, m_pPool[hle.m_poolIndex]->m_iBlockSize);
	int index = m_pPool[hle.m_poolIndex]->m_iFreeBlockIndex - 1;
	m_pPool[hle.m_poolIndex]->m_iFreeBlockIndex--;
	if (index == -1)
	{
		m_pPool[hle.m_poolIndex]->m_iFreeBlockIndex = MEMORY_POOL_CONFIG[hle.m_poolIndex][1] - 1;
		index = MEMORY_POOL_CONFIG[hle.m_poolIndex][1] - 1;
	}
	m_pPool[hle.m_poolIndex]->m_pFreeList[index] = hle.m_blockIndex;
	m_pPool[hle.m_poolIndex]->m_iFreeBlockNum++;
}

// Get the raw address stored with reference to handle
void* MemoryManager::GetMemoryAddressFromHandle(Handle hle)
{
	if (hle.m_counter == 0)
	{
		return nullptr;
	}
	assert((((uint64_t)m_pPool[hle.m_poolIndex] + sizeof(unsigned int) * 4 + sizeof(unsigned int) * MEMORY_POOL_CONFIG[hle.m_poolIndex][1] + m_pPool[hle.m_poolIndex]->m_iBlockSize * hle.m_blockIndex)) % 16 == 0); // temp check
	return (void*)((uint64_t)m_pPool[hle.m_poolIndex] + sizeof(unsigned int) * 4 + sizeof(unsigned int) * MEMORY_POOL_CONFIG[hle.m_poolIndex][1] + m_pPool[hle.m_poolIndex]->m_iBlockSize * hle.m_blockIndex);
}

// Return a aligned address according to the alignment
void* MemoryManager::alignedAddress(void* ptr)
{
	unsigned int adjustment = (unsigned int)ptr % MEMORY_ALIGNMENT;
	if (adjustment)
	{
		//unsigned int* intptr = (unsigned int*) ptr;
		//intptr[0] = adjustment;
		return (void *)((unsigned int)ptr + MEMORY_ALIGNMENT - adjustment);
	}
	else
	{
		return ptr;
	}
}

};