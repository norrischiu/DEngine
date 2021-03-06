#ifndef MY_ARRAY_H_
#define MY_ARRAY_H_

// Engine include
#include "Memory\Handle.h"

namespace DE
{

/*
*	TEMPLATE CLASS: MyArray
*	T is the class of the item to be stored
*	This is the default contiguous array to be used in DEngine, it behaves 
*	in similar way as std::vector, it doubles its capacity when the size 
*	exceeded the capacity. It is strongly recommended to use this class 
*	instead of std::vector or other STL data structure to keep clean 
*	record of memory usage and avoid dynamic memory allocation during gameplay
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
	MyArray(size_t capacity, bool persistent = false)
	{
		m_iSize = 0;
		m_iCapacity = capacity;
		m_bPersistent = persistent;
		if (m_iCapacity > 0)
		{
			m_hElements.Set(sizeof(T) * capacity);
			memset(m_hElements.Raw(), NULL, sizeof(T) * capacity);
		}
	}

	/********************************************************************************
	*	--- Copy Constructor:
	*	MyArray(MyArray&)
	*	This constructor will construct an handle array by copying an existing array,
	*	the underlying mechanics is to simply copy all member variables and imcrement
	*	the counter at handle
	*
	*	--- Parameters:
	*	@ other: the other MyArray object
	********************************************************************************/
	MyArray(MyArray& other)
	{
		m_hElements = other.m_hElements;
		m_hElements.m_counter++;
		m_iSize = other.m_iSize;
		m_iCapacity = other.m_iCapacity;
	}

	/********************************************************************************
	*	--- Destructor:
	*	~MyArray()
	*	This destructor will free the memory used by this array's items
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	~MyArray()
	{
		if (!m_bPersistent)
		{
			m_hElements.Free();
		}
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
	inline size_t Size()
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
		if (m_iCapacity == 0)
		{
			Resize(1);
		}
		if (m_iSize >= m_iCapacity)
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
		unsigned int oldCapacity = m_iCapacity;
		m_iCapacity = capacity;
		Handle hNewElements(sizeof(T) * capacity);
		if (oldCapacity > 0)
		{
			memcpy(hNewElements.Raw(), m_hElements.Raw(), sizeof(T) * m_iSize);
			m_hElements.Free();
		}
		m_hElements = hNewElements;
	}

	/********************************************************************************
	*	--- Function:
	*	Pop()
	*	This function will virtually remove the last item by decrementing the size
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Pop()
	{
		m_iSize--;
	}

	/********************************************************************************
	*	--- Function:
	*	Clear(size_t)
	*	This function will clear the items in this array, leaving the capacity and
	*	allocated memory unchanged
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Clear()
	{
		if (m_iCapacity > 0)
		{
			memset(m_hElements.Raw(), NULL, sizeof(T) * m_iSize);
		}
		m_iSize = 0;
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
	*	@ T&: the item located at the index
	********************************************************************************/
	T& operator[](const int index)
	{
		return ((T*)m_hElements.Raw())[index];
	}

	/********************************************************************************
	*	--- Function:
	*	Raw()
	*	This function will return the pointer to the beginning of this array
	*
	*	--- Parameters:
	*	@ void:
	*
	*	--- Return:
	*	@ T*: the pointer of the first element
	********************************************************************************/
	T* Raw()
	{
		return ((T*)m_hElements.Raw());
	}

private:

	Handle						m_hElements;		// the handle array containing the exact data
	unsigned int				m_iSize;		// the current size of array and also the index to the next element
	unsigned int				m_iCapacity;	// the current capacity
	bool						m_bPersistent;	// if TRUE will not free the memory when destruct, if FALSE otherwise
};

} // namespace DE

#endif // !MY_ARRAY_H_