#ifndef MY_ARRAY_H_
#define MY_ARRAY_H_

// Engine include
#include "Memory\Handle.h"

namespace DE
{

/*
*	TEMPLATE CLASS: MyArray
*	T is the class of the item class to be stored
*	This is the default array to be used in DEngine, it behaves in similar
*	way as std::vector, it doubles its capacity when the size exceeded the 
*	capacity. It is strongly recommended to use this class instead of 
*	std::vector or other STL data structure to keep clean record of memory
*	usage and avoid dynamic memory allocation during gameplay
*/
template <class T> 
class MyArray
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	MyArray(size_t)
	*	This constructor will construct an handle array with give capacity. Enough
	*	capacity should be allocated to fit the possible size of the array in order
	*	to avoid memory reallocation
	*
	*	--- Parameters:
	*	@ capacity: the initial capacity of this array
	********************************************************************************/
	MyArray(size_t capacity)
	{
		m_iSize = 0;
		m_iCapacity = capacity;
		m_hElements.Set(sizeof(T) * capacity);
	}

	/********************************************************************************
	*	--- Function:
	*	Size()
	*	This function will return the current size of this array
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ size_t: the current size (number of items) of this array
	********************************************************************************/
	size_t Size()
	{
		return m_iSize;
	}

	/********************************************************************************
	*	--- Function:
	*	Add(T)
	*	This function will add an element as a handle at the back of this array
	*
	*	--- Parameters:
	*	@ item: the item to be added, should be the same as the template class
	*
	*	--- Return:
	*	@ void:
	********************************************************************************/
	void Add(T item)
	{
		if (m_iSize > m_iCapacity)
		{
			Resize(m_iCapacity * 2);
		}
		((T*)m_hElements.Raw())[m_iSize] = item;
		m_iSize++;
	}

	/********************************************************************************
	*	--- Function:
	*	Resize(size_t)
	*	This function will resize the array's capacity
	*
	*	--- Parameters:
	*	@ capacity: the new capacity
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Resize(size_t capacity)
	{
		Handle hNewElements(sizeof(T) * capacity);
		memcpy(hNewElements.Raw(), m_hElements.Raw(), sizeof(T) * m_iSize);
		m_hElements.Free();
		m_hElements = hNewElements;
	}

	/********************************************************************************
	*	--- Function:
	*	Clear(size_t)
	*	This function will release the memory used by this array
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Clear()
	{
		m_hElements.Free();
	}

	/********************************************************************************
	*	--- Function:
	*	operator[](const int)
	*	This function will return the element located at the index-th element of 
	*	this array
	*
	*	--- Parameters:
	*	@ index: the element index
	*
	*	--- Return:
	*	@ T: the item located at the index
	********************************************************************************/
	T operator[](const int index)
	{
		return ((T*)m_hElements.Raw())[index];
	}

private:

	Handle						m_hElements;		// the handle array containing the exact data
	unsigned int				m_iSize;		// the current size of array and also the index to the next element
	unsigned int				m_iCapacity;	// the current capacity
};

} // namespace DE

#endif // !MY_ARRAY_H_