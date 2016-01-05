#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include <queue>

enum EventQueueType
{
	INPUT_EVENT,
	GAME_EVENT
};

class EventQueue
{

public:
	void Add(Event* evt)
	{
		m_inputQueue.push(evt);
	}

	Event* Front()
	{
		if (!m_inputQueue.empty())
		{
			Event* pEvt = m_inputQueue.front();
			return pEvt;
		}
		return nullptr;
	}

	void Pop()
	{
		m_inputQueue.pop();
	}

	static EventQueue* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new EventQueue;
		}
		return m_pInstance;
	}

private:
	static EventQueue*					m_pInstance;

	std::queue<Event*>					m_inputQueue;

	std::queue<Event*>					m_gameQueue;
};

#endif // !EVENT_QUEUE

