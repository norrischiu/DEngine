#ifndef EVENT_H
#define EVENT_H

struct Event
{
	Event(unsigned int ID)
		:m_ID(ID)
	{};

	unsigned int m_ID;
};

#endif // !EVENT_H

