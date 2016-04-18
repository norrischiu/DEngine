#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "Event/EventQueue.h"
#include "Event/InputEvent.h"
#include <unordered_map>

#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_A 0x41
#define VK_D 0x44
#define VK_W 0x57
#define VK_S 0x53
#define VK_SHIFT 0x10

namespace DE
{

class Keyboard
{
	struct State
	{
		std::unordered_map<int, bool> Keys = std::unordered_map<int, bool>
			({
				{ VK_W, false },
				{ VK_S, false },
				{ VK_A, false },
				{ VK_D, false },
				{ VK_SHIFT, false }
		});
	};

public:

	static void Update(float deltaTime)
	{
		for (auto itr : m_currState.Keys)
		{
			if (itr.second & m_lastState.Keys[itr.first])
			{
				if (itr.first == VK_W)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_W_Hold_Event));
					new (hEvt) Key_W_Hold_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_S)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_S_Hold_Event));
					new (hEvt) Key_S_Hold_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_A)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_A_Hold_Event));
					new (hEvt) Key_A_Hold_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_D)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_D_Hold_Event));
					new (hEvt) Key_D_Hold_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_SHIFT)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_LSHIFT_Hold_Event));
					new (hEvt) Key_LSHIFT_Hold_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
			}
			if (itr.second & !m_lastState.Keys[itr.first])
			{
				if (itr.first == VK_W)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_W_Press_Event));
					new (hEvt) Key_W_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_S)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_S_Press_Event));
					new (hEvt) Key_S_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_A)
				{
					Handle hEvt(sizeof(Key_A_Press_Event));
					new (hEvt) Key_A_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_D)
				{
					Handle hEvt(sizeof(Key_D_Press_Event));
					new (hEvt) Key_D_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_SHIFT)
				{
					Handle hEvt(sizeof(Key_LSHIFT_Press_Event));
					new (hEvt) Key_LSHIFT_Press_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
			}
			if (!itr.second & m_lastState.Keys[itr.first])
			{
				if (itr.first == VK_W)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_W_Release_Event));
					new (hEvt) Key_W_Release_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_S)
				{
					Handle hEvt = MemoryManager::GetInstance()->Allocate(sizeof(Key_S_Release_Event));
					new (hEvt) Key_S_Release_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_A)
				{
					Handle hEvt(sizeof(Key_A_Release_Event));
					new (hEvt) Key_A_Release_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
				if (itr.first == VK_D)
				{
					Handle hEvt(sizeof(Key_D_Release_Event));
					new (hEvt) Key_D_Release_Event;
					EventQueue::GetInstance()->Add(hEvt, INPUT_EVENT);
				}
			}
		}
		m_lastState = m_currState;
	}

	static void SetInputKey(int virtualKey, bool flag)
	{
		m_currState.Keys[virtualKey] = flag;
	}

private:

	static State m_currState;
	static State m_lastState;
};

};

#endif // !KEYBOARD_H_
