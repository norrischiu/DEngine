// MemoryManager.h: singleton class of memory manager
#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include "MemoryPool.h"
#include <iostream>
class Handle;

const unsigned int MEMORY_POOL_NUM = 22;
const unsigned int MEMORY_POOL_CONFIG[][2] = 
{
	// already 16-byte aligned
	// block size, number of block
	{ 16,       65536 },  //
	{ 32,       4096 },  //
	{ 48,       4096 },  //
	{ 64,       2048 },  //
	{ 80,       2048 },  //
	{ 96,       2048 },  //
	{ 112,      2048 },  //
	{ 128,      2048 },  //
	{ 256,      2048 },  //
	{ 512,      2048 },  //
	{ 1024,     2048 },  //
	{ 2048,     2048 },  //
	{ 4096,     512 },  //
	{ 8192,     512 },  //
	{ 16384,    1024 },  //
	{ 32768,    512 },   //
	{ 65536,    256 },   //
	{ 131072,   32 },   //
	{ 262144,   16 },    //
	{ 524288,   8 },    // 
	{ 1048576,  4 },    // 
	{ 2097152,  2 },     //
	{ 4194304,  1 },     //
};

class MemoryManager
{

public:
	
	void ConstructDefaultPool();
	
	void Defragment();

	void Destruct();

	// Allocate a memory block of size from the pool, return the handle
	Handle Allocate(size_t size);

	// Free the memory block back to the pool
	void Free(Handle hle);

	// Get the raw address stored with reference to handle
	void* GetMemoryAddressFromHandle(Handle hle);

	// Return singleton instance
	static MemoryManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new MemoryManager();
		return m_pInstance;
	};

	void Print()
	{
		std::cout << "Num of free:" << m_pPool[0]->m_iFreeBlockNum << "\n";
		std::cout << "Index of free:" << m_pPool[0]->m_iFreeBlockIndex << "\n";
	}

private:
	// Default constructor
	MemoryManager(){}

	// Singleton instance
	static MemoryManager*					m_pInstance;

	// Raw heap start address
	void*									m_pRawHeapStart;
	
	// All memory blocks' pools
	MemoryPool*								m_pPool[MEMORY_POOL_NUM];

	void* alignedAddress(void* ptr);
};

#endif
