#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "Event.h"

namespace EventID 
{
	enum
	{
		Mouse_Move_Event,
		Key_W_Press_Event,
		Key_W_Release_Event,
		Key_W_Hold_Event,
		Key_S_Hold_Event,
		Key_A_Hold_Event,
		Key_D_Hold_Event,
	};
}


struct Mouse_Move_Event : public Event
{
	Mouse_Move_Event()
		:Event(EventID::Mouse_Move_Event)
	{};
public:
	long cursorPosChange[2];
};

struct Key_W_Press_Event : public Event
{
	Key_W_Press_Event()
		:Event(EventID::Key_W_Press_Event)
	{};
};

struct Key_W_Release_Event : public Event
{
	Key_W_Release_Event()
		:Event(EventID::Key_W_Release_Event)
	{};
};

struct Key_W_Hold_Event : public Event
{
	Key_W_Hold_Event()
		:Event(EventID::Key_W_Hold_Event)
	{};
};

struct Key_S_Hold_Event : public Event
{
	Key_S_Hold_Event()
		:Event(EventID::Key_S_Hold_Event)
	{};
};

struct Key_A_Hold_Event : public Event
{
	Key_A_Hold_Event()
		:Event(EventID::Key_A_Hold_Event)
	{};
};

struct Key_D_Hold_Event : public Event
{
	Key_D_Hold_Event()
		:Event(EventID::Key_D_Hold_Event)
	{};
};

#endif
