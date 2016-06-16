#ifndef EVENT_H
#define EVENT_H

#include "Memory\Handle.h"

namespace DE
{

/*
*	STRUCT: Event
*	The base class of all events in the engine and gameplay
*/
struct Event
{

	CUSTOM_MEMORY_DEFINE();

	/********************************************************************************
	*	--- Constructor:
	*	Event(unsigned int)
	*	This constructor will construct an event with the given ID
	*
	*	--- Parameters:
	*	@ ID: the event ID
	********************************************************************************/
	Event(unsigned int ID)
		:m_ID(ID)
	{};

	unsigned int m_ID;		// the ID to identify the event type
};

};
#endif // !EVENT_H

