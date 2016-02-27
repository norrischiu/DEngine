#ifndef PLAYER_EVENT_H
#define PLAYER_EVENT_H

#include "DEngine\Event\Event.h"

namespace EventID
{
	enum
	{
		Player_Walk_START_Event,
		Player_Walk_END_Event,
		Player_Attack_1_START_Event,
		Player_Attack_1_END_Event,
		Animation_clip_END_Event
	};
}


struct Player_Walk_START_Event : public Event
{
	Player_Walk_START_Event()
		:Event(EventID::Player_Walk_START_Event)
	{};
};

struct Player_Walk_END_Event : public Event
{
	Player_Walk_END_Event()
		:Event(EventID::Player_Walk_END_Event)
	{};
};

struct Player_Attack_1_START_Event : public Event
{
	Player_Attack_1_START_Event()
		:Event(EventID::Player_Attack_1_START_Event)
	{};
};

struct Player_Attack_1_END_Event : public Event
{
	Player_Attack_1_END_Event()
		:Event(EventID::Player_Attack_1_END_Event)
	{};
};

#endif // !PLAYER_EVENT_H
