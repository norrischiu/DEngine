#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "Memory\Handle.h"
#include <queue>
#include <assert.h>

enum EventQueueType
{
	INPUT_EVENT,
	GAME_EVENT
};

class EventQueue
{

public:
	void Add(Handle evt)
	{
		m_inputQueue.push(evt);
	}

	Handle Front()
	{
		return m_inputQueue.front();
	}

	void Pop()
	{
		m_inputQueue.front().Free();
		m_inputQueue.pop();
	}

	bool Empty()
	{
		return m_inputQueue.empty();
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

	std::queue<Handle>					m_inputQueue;

	std::queue<Handle>					m_gameQueue;
};

#endif // !EVENT_QUEUE

