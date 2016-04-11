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

	void FrontToBack(int type)
	{
		if (type == INPUT_EVENT)
		{
			Handle hEvt = m_inputQueue.front();
			m_inputQueue.pop();
			m_inputQueue.push(hEvt);
		}
		else
		{
			Handle hEvt = m_gameQueue.front();
			m_gameQueue.pop();
			m_gameQueue.push(hEvt);
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

	int Size(int type)
	{
		if (type == INPUT_EVENT)
		{
			return m_inputQueue.size();
		}
		else
		{
			return m_gameQueue.size();
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

