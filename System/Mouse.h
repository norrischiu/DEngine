#ifndef MOUSE_H_
#define MOUSE_H_

#include "Event/EventQueue.h"
#include "Event/InputEvent.h"

class Mouse
{
	struct State
	{
		long cursorPos[2] = { 0 };
	};

public:

	static void Update(float deltaTime)
	{
		Mouse_Move_Event* evt = new Mouse_Move_Event;
		evt->cursorPosChange[0] = m_currState.cursorPos[0] - m_lastState.cursorPos[0];
		evt->cursorPosChange[1] = m_currState.cursorPos[1] - m_lastState.cursorPos[1];
		EventQueue::GetInstance()->Add(evt);
		m_lastState = m_currState;
	}

	static void SetCursorPos(long x, long y)
	{
		m_currState.cursorPos[0] = x;
		m_currState.cursorPos[1] = y;
	}

	static State m_currState;
	static State m_lastState;
};

Mouse::State Mouse::m_currState;
Mouse::State Mouse::m_lastState;

#endif // !KEYBOARD_H_
