#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

// Engine include
#include "Event.h"

// C++ include
#include <queue>
#include <assert.h>

namespace DE
{

/*
*	ENUM: EventQueueType
*	The type of event in the queue
*/
enum EventQueueType
{
	INPUT_EVENT, // event generated from mouse/keyboard
	GAME_EVENT	// event generated from gameplay
};

/*
*	CLASS: EventQueue
*	The event queue is a singleton which contains all events with in
*	the engine in a First-In-First-Out manner, currently is divided 
*	into two types: input and game
*/
class EventQueue
{

public:

	/********************************************************************************
	*	--- Function:
	*	Add(Handle, int)
	*	This function will add the handle pointing to the event into the queue
	*	specified by the type
	*
	*	--- Parameters:
	*	@ evt: the handle pointing to the event
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	FrontToBack(int)
	*	This function will pop the front event and push it back to the end of the
	*	queue, this is used to filter event that is not handled by a certain handler
	*
	*	--- Parameters:
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	Front(int)
	*	This function will return the front event according to the type
	*
	*	--- Parameters:
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ Handle: the handle pointing to the front most event
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	Pop(int)
	*	This function will remove the front event according to the type
	*
	*	--- Parameters:
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	Size(int)
	*	This function will return the size of the queue of that type
	*
	*	--- Parameters:
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ int: the size of the queue
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	Size(int)
	*	This function will check if the queue of that type is empty
	*
	*	--- Parameters:
	*	@ type: the type of the event
	*
	*	--- Return:
	*	@ bool: True if it is empty; False if it contains at least one event
	********************************************************************************/
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

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of event queue
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ EventQueue*: the singleton instance
	********************************************************************************/
	static EventQueue* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new EventQueue;
		}
		return m_pInstance;
	}

private:

	static EventQueue*					m_pInstance;	// singleton instance
	std::queue<Handle>					m_inputQueue;	// queue of input event
	std::queue<Handle>					m_gameQueue;	// queue of game event
};

};
#endif // !EVENT_QUEUE

