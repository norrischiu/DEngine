#ifndef MOUSE_H_
#define MOUSE_H_

// Engine include
#include "Event/EventQueue.h"
#include "Event/InputEvent.h"

// Window includes
#include <Windows.h>

namespace DE
{

class Mouse
{
	struct State
	{
		void operator=(const State& other)
		{
			cursorPos[0] = other.cursorPos[0];
			cursorPos[1] = other.cursorPos[1];
			Buttons[0] = other.Buttons[0];
			Buttons[1] = other.Buttons[1];
		}
		long cursorPos[2] = { 0 };
		bool Buttons[2] = { false, false };
	};

public:

	static void Update(float deltaTime)
	{
		// Mouse movement
		Handle hEvt(sizeof(Mouse_Move_Event));
		Mouse_Move_Event* evt = new (hEvt) Mouse_Move_Event;
		evt->cursorPosChange[0] = m_currState.cursorPos[0] - m_lastState.cursorPos[0];
		evt->cursorPosChange[1] = m_currState.cursorPos[1] - m_lastState.cursorPos[1];
		evt->cursorPos[0] = m_currState.cursorPos[0];
		evt->cursorPos[1] = m_currState.cursorPos[1];
		EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);

		// Mouse buttons
		for (int i = 0; i < 2; ++i)
		{
			if (m_currState.Buttons[i] & !m_lastState.Buttons[i])
			{
				if (i == MK_LBUTTON)
				{
					Handle hButtonEvt(sizeof(Mouse_Left_Press_Event));
					new (hButtonEvt) Mouse_Left_Press_Event;
					EventQueue::GetInstance()->Add(hButtonEvt, INPUT_EVENT);
				}
			}
			if (!m_currState.Buttons[i] & m_lastState.Buttons[i])
			{
				if (i == MK_LBUTTON)
				{
					Handle hButtonEvt(sizeof(Mouse_Left_Release_Event));
					new (hButtonEvt) Mouse_Left_Release_Event;
					EventQueue::GetInstance()->Add(hButtonEvt, INPUT_EVENT);
				}
			}
		}
		m_lastState = m_currState;
	}

	static void SetMousePos(long x, long y)
	{
		m_currState.cursorPos[0] = x;
		m_currState.cursorPos[1] = y;
	}

	static void SetButton(int virtualKey, bool flag)
	{
		m_currState.Buttons[virtualKey] = flag;
	}

	static void ResetWindowCursorPos()
	{
		POINT p;
		p.x = 512.0f;
		p.y = 384.0f;
		ClientToScreen(GetActiveWindow(), &p);
		SetCursorPos(p.x, p.y);
		m_lastState.cursorPos[0] = 512.0f;
		m_lastState.cursorPos[1] = 384.0f;
	}

private:

	static State m_currState;
	static State m_lastState;
};

};
#endif // !KEYBOARD_H_
