#ifndef MY_HASHMAP_H_
#define MY_HASHMAP_H_

// Engine include
#include "Memory\Handle.h"
#include "MyArray.h"

// C++ include
#include <functional>

namespace DE
{

/*
*	TEMPLATE CLASS: MyHashMap
*	T is the class of the item to be stored
*	This is the default C string hash map to be used in DEngine, it behaves in similar
*	way as std::unordered_map. The key is a C string (i.e. char*) and the item is 
*	any class provided. This uses a djb2 hash function and by default allocated 157 
*	spaces for the item. This uses linear probing to resolve collision. It is strongly 
*	recommended to use this class instead of std::unordered_map or other STL data 
*	structure to keep clean record of memory usage and avoid dynamic memory allocation 
*	during gameplay
*	Use Add to insert a new pair, DO NOT use operator[] to insert
*/
template <class T>
class MyHashMap
{

public:

	/*
	*	CLASS: MyHashMap<T>::HashMapPair
	*	T is the class of the item to be stored
	*	The hash map pair is a struct consists of a C string as the key, and the template
	*	class T as the item
	*/
	struct HashMapPair
	{
		/********************************************************************************
		*	--- Constructor:
		*	HashMapPair(const char*, T)
		*	This constructor will construct an hash map pair with the give C string as
		*	key and the other as item
		*
		*	--- Parameters:
		*	@ cString: the key of this pair
		*	@ item: the item of this pair
		********************************************************************************/
		HashMapPair(const char* cString, T item)
			: m_Item(item)
		{
			strcpy(m_Key, cString);
		}

		/********************************************************************************
		*	--- Assignment Operator:
		*	operator=(HashMapPair&)
		*	This operator will assign the key and item from another pair to the caller
		*	pair
		*
		*	--- Parameters:
		*	@ other: the other existing hash map pair
		*
		*	--- Return:
		*	@ HashMapPair&: the result hash map pair after assignment
		********************************************************************************/
		HashMapPair& operator=(HashMapPair& other)
		{
			strcpy(this->m_Key, other.m_Key);
			this->m_Item = other.m_Item;
			return *this;
		}

		char				m_Key[256];		// this pair's key
		T					m_Item;		// thie pair's item
	};

	/********************************************************************************
	*	--- Constructor:
	*	MyHashMap(size_t)
	*	This constructor will construct an hash map with an MyArray as the container.
	*	By default the size is 157, it can be extended to support dynamic size
	*
	*	--- Parameters:
	*	@ size: the size of this hash map
	********************************************************************************/
	MyHashMap(size_t size = 157)
		:m_iArray(size)
	{
		m_iMapSize = size;
		m_iSize = 0;
	}

	/********************************************************************************
	*	--- Destructor:
	*	~MyArray()
	*	This destructor will free the memory used by this hash map's items
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	~MyHashMap()
	{
	}

	/********************************************************************************
	*	--- Function:
	*	Size()
	*	This function will return the current size of this hash map
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ size_t: the current size (number of items) of this hash map
	********************************************************************************/
	inline size_t Size()
	{
		return m_iSize;
	}

	/********************************************************************************
	*	--- Function:
	*	Hash(const char*)
	*	This function will hash a C string key to an index using Bernstein's hash
	*	function djb2 (universal string hashing with initial value 5381 and "a" equals
	*	33).
	*
	*	--- Parameters:
	*	@ key: the C string key
	*
	*	--- Return:
	*	@ unsigned int: hashed index of this key
	********************************************************************************/
	unsigned int Hash(const char* key)
	{
		unsigned int hashed = 5381;
		while (*key)
		{
			hashed = hashed * 33 + (*key);
			key++;
		}
		return hashed % m_iMapSize;
	}

	/********************************************************************************
	*	--- Function:
	*	Add(const char*, T)
	*	This function will add a pair to this hash map
	*
	*	--- Parameters:
	*	@ key: the key paired with this item
	*	@ item: the item to be added, should be the same as the template class
	*
	*	--- Return:
	*	@ void:
	********************************************************************************/
	void Add(const char* key, T item)
	{
		unsigned int index = Hash(key);
		HashMapPair pair(key, item);
		m_iArray[index] = pair;
		m_iSize++;
	}

	/********************************************************************************
	*	--- Function:
	*	Remove(const char*)
	*	This function will remove a pair with the given key
	*
	*	--- Parameters:
	*	@ key: the key paired with this item
	*
	*	--- Return:
	*	@ void:
	********************************************************************************/
	void Remove(const char* key)
	{
		unsigned int index = Hash(key);
		while (strcmp(m_iArray[index].m_Key, key) != 0)
		{
			if (index >= m_iMapSize)
			{
				assert(false && "this key does not exist");
			}
			index++;
		}
		memset(m_iArray[index].m_Key, NULL, 256);
		m_iSize--;
	}

	/********************************************************************************
	*	--- Function:
	*	Clear()
	*	This function will clear the items in this hash map, leaving the allocated
	*	memory unchanged
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Clear()
	{
		m_iArray.Clear();
		m_iSize = 0;
	}

	/********************************************************************************
	*	--- Function:
	*	Contain(const char*)
	*	This function will check if this hash map contains a pair with the given key
	*
	*	--- Parameters:
	*	@ key: the C string as the key
	*
	*	--- Return:
	*	@ bool: True if this hash map contains pair with this key; False if this hash
	*	map does NOT contain a pair with this key
	********************************************************************************/
	bool Contain(const char* key)
	{
		unsigned int index = Hash(key);
		const char* a = m_iArray[index].m_Key;
		while (strlen(m_iArray[index].m_Key) != 0 && index < m_iMapSize)
		{
			if (strcmp(m_iArray[index].m_Key, key) == 0)
			{
				return true;
			}
			index++;
		}
		return false;
	}

	/********************************************************************************
	*	--- Function:
	*	operator[](const char*)
	*	This function will return the element associated with this key. Currently
	*	assert if key does not exist
	*
	*	--- Parameters:
	*	@ key: the C string key
	*
	*	--- Return:
	*	@ T&: the item associated with the key
	********************************************************************************/
	T& operator[](const char* key)
	{
		unsigned int index = Hash(key);
		while (strcmp(m_iArray[index].m_Key, key) != 0)
		{
			if (index >= m_iMapSize)
			{
				assert(false && "this key does not exist");
			}
			index++;
		}
		return m_iArray[index].m_Item;
	}

	/********************************************************************************
	*	--- Function:
	*	Raw()
	*	This function will return the pointer to the beginning of the array container
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ T*: the pointer of the first element
	********************************************************************************/
	T* Raw()
	{
		return m_iArray.Raw();
	}

	/********************************************************************************
	*	--- Template Function:
	*	F is the class with a operator() defined
	*	ForEachItem(F)
	*	This function will run the given function with all items with non empty key
	*
	*	--- Parameters:
	*	@ function: the function that will be run for all items, can be a lambda,
	*	function pointer or function object
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	template <typename F>
	void ForEachItem(F function)
	{
		for (int i = 0; i < m_iMapSize; ++i)
		{
			if (strlen(m_iArray[i].m_Key) != 0)
			{
				function(m_iArray[i].m_Item);
			}
		}
	}

	/********************************************************************************
	*	--- Template Function:
	*	F is the class with a operator() defined
	*	ForEachPair(F)
	*	This function will run the given function with all key-item pairs with 
	*	non empty key
	*
	*	--- Parameters:
	*	@ function: the function that will be run for all pairs, can be a lambda,
	*	function pointer or function object
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	template <typename F>
	void ForEachPair(F function)
	{
		for (int i = 0; i < m_iMapSize; ++i)
		{
			if (strlen(m_iArray[i].m_Key) != 0)
			{
				function(m_iArray[i]);
			}
		}
	}

private:

	MyArray<HashMapPair>				m_iArray;		// the array containing the pair
	unsigned int						m_iSize;		// the current number of items
	unsigned int						m_iMapSize;		// the allocated size of the whole map
};

} // namespace DE

#endif // !MY_HASHMAP_H_
