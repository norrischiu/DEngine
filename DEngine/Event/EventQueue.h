#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include <queue>
#include <assert.h>

namespace DE
{

enum EventQueueType
{
	INPUT_EVENT,
	GAME_EVENT
};

class EventQueue
{

public:
	void Add(Handle evt, int type)
	{
		if (type == INPUT_EVENT)
		{
			m_inputQueue.push(evt);
		}
		else
		{
			m_gameQueue.push(evt);
		}
	}

	Handle Front(int type)
	{
		if (type == INPUT_EVENT)
		{
			return m_inputQueue.front();
		}
		else
		{
			return m_gameQueue.front();
		}
	}

	void Pop(int type)
	{
		if (type == INPUT_EVENT)
		{
			m_inputQueue.front().Free();
			m_inputQueue.pop();
		}
		else
		{
			m_gameQueue.front().Free();
			m_gameQueue.pop();
		}
	}

	bool Empty(int type)
	{
		if (type == INPUT_EVENT)
		{
			return m_inputQueue.empty();
		}
		else
		{
			return m_gameQueue.empty();
		}
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

};
#endif // !EVENT_QUEUE

