#ifndef MOUSE_H_
#define MOUSE_H_

// Engine include
#include "Event/EventQueue.h"
#include "Event/InputEvent.h"

// C++ include
#include <unordered_map>

// Window includes
#include <Windows.h>

namespace DE
{

class Mouse
{
	struct State
	{
		long cursorPos[2] = { 0 };
		std::unordered_map<int, bool> Buttons = std::unordered_map<int, bool>
			({
				{ MK_LBUTTON, false },
				{ MK_RBUTTON, false },
			});
	};

public:

	static void Update(float deltaTime)
	{
		// Mouse movement
		hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Mouse_Move_Event));
		Mouse_Move_Event* evt = new (hEvt) Mouse_Move_Event;
		evt->cursorPosChange[0] = m_currState.cursorPos[0] - m_lastState.cursorPos[0];
		evt->cursorPosChange[1] = m_currState.cursorPos[1] - m_lastState.cursorPos[1];
		evt->cursorPos[0] = m_currState.cursorPos[0];
		evt->cursorPos[1] = m_currState.cursorPos[1];
		EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);

		// Mouse buttons
		for (auto itr : m_currState.Buttons)
		{
			if (itr.second & !m_lastState.Buttons[itr.first])
			{
				if (itr.first == MK_LBUTTON)
				{
					Handle hEvt(sizeof(Mouse_Left_Press_Event));
					new (hEvt) Mouse_Left_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
			}
			if (!itr.second & m_lastState.Buttons[itr.first])
			{
				if (itr.first == MK_LBUTTON)
				{
					Handle hEvt(sizeof(Mouse_Left_Release_Event));
					new (hEvt) Mouse_Left_Release_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
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

	static State m_currState;
	static State m_lastState;

	static Handle hEvt;
};

};
#endif // !KEYBOARD_H_
