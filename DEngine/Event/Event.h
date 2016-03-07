#ifndef EVENT_H
#define EVENT_H

#include "Memory\Handle.h"

namespace DE
{

struct Event
{

	CUSTOM_MEMORY_DEFINE();

	Event(unsigned int ID)
		:m_ID(ID)
	{};

	unsigned int m_ID;
};

};
#endif // !EVENT_H

