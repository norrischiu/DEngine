#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "Event.h"

namespace DE
{

namespace InputEventID
{
	enum
	{
		Mouse_Move_Event = 0x00,
		Mouse_Left_Press_Event,
		Mouse_Left_Hold_Event,
		Mouse_Left_Release_Event,
		Mouse_Right_Press_Event,
		Mouse_Right_Release_Event,
		Key_W_Press_Event,
		Key_W_Release_Event,
		Key_S_Press_Event,
		Key_S_Release_Event,
		Key_D_Press_Event,
		Key_D_Release_Event,
		Key_A_Press_Event,
		Key_A_Release_Event,
		Key_W_Hold_Event,
		Key_S_Hold_Event,
		Key_A_Hold_Event,
		Key_D_Hold_Event,
		Key_LSHIFT_Hold_Event,
		Key_LSHIFT_Press_Event
	};
}


struct Mouse_Move_Event : public Event
{
	Mouse_Move_Event()
		:Event(InputEventID::Mouse_Move_Event)
	{};
public:
	long cursorPosChange[2];
	long cursorPos[2];
};

struct Mouse_Left_Press_Event : public Event
{
	Mouse_Left_Press_Event()
		:Event(InputEventID::Mouse_Left_Press_Event)
	{};
};

struct Mouse_Left_Release_Event : public Event
{
	Mouse_Left_Release_Event()
		:Event(InputEventID::Mouse_Left_Release_Event)
	{};
};

struct Mouse_Left_Hold_Event : public Event
{
	Mouse_Left_Hold_Event()
		:Event(InputEventID::Mouse_Left_Hold_Event)
	{};
};

struct Mouse_Right_Press_Event : public Event
{
	Mouse_Right_Press_Event()
		:Event(InputEventID::Mouse_Right_Press_Event)
	{};
};

struct Mouse_Right_Release_Event : public Event
{
	Mouse_Right_Release_Event()
		:Event(InputEventID::Mouse_Right_Release_Event)
	{};
};

struct Key_W_Press_Event : public Event
{
	Key_W_Press_Event()
		:Event(InputEventID::Key_W_Press_Event)
	{};
};

struct Key_W_Release_Event : public Event
{
	Key_W_Release_Event()
		:Event(InputEventID::Key_W_Release_Event)
	{};
};

struct Key_W_Hold_Event : public Event
{
	Key_W_Hold_Event()
		:Event(InputEventID::Key_W_Hold_Event)
	{};
};

struct Key_S_Press_Event : public Event
{
	Key_S_Press_Event()
		:Event(InputEventID::Key_S_Press_Event)
	{};
};

struct Key_S_Release_Event : public Event
{
	Key_S_Release_Event()
		:Event(InputEventID::Key_S_Release_Event)
	{};
};

struct Key_S_Hold_Event : public Event
{
	Key_S_Hold_Event()
		:Event(InputEventID::Key_S_Hold_Event)
	{};
};

struct Key_A_Hold_Event : public Event
{
	Key_A_Hold_Event()
		:Event(InputEventID::Key_A_Hold_Event)
	{};
};

struct Key_A_Press_Event : public Event
{
	Key_A_Press_Event()
		:Event(InputEventID::Key_A_Press_Event)
	{};
};

struct Key_A_Release_Event : public Event
{
	Key_A_Release_Event()
		:Event(InputEventID::Key_A_Release_Event)
	{};
};

struct Key_D_Hold_Event : public Event
{
	Key_D_Hold_Event()
		:Event(InputEventID::Key_D_Hold_Event)
	{};
};

struct Key_D_Press_Event : public Event
{
	Key_D_Press_Event()
		:Event(InputEventID::Key_D_Press_Event)
	{};
};

struct Key_D_Release_Event : public Event
{
	Key_D_Release_Event()
		:Event(InputEventID::Key_D_Release_Event)
	{};
};

struct Key_LSHIFT_Hold_Event : public Event
{
	Key_LSHIFT_Hold_Event()
		:Event(InputEventID::Key_LSHIFT_Hold_Event)
	{};
};

struct Key_LSHIFT_Press_Event : public Event
{
	Key_LSHIFT_Press_Event()
		:Event(InputEventID::Key_LSHIFT_Press_Event)
	{};
};



}

#endif
