#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "DEngine\Event\Event.h"

namespace GameEventID
{
	enum
	{
		Player_Walk_START_Event = 0xFFF,
		Player_Walk_END_Event,
		Player_Attack_1_START_Event,
		Player_Attack_1_END_Event,
		Animation_clip_END_Event
	};
}


struct Player_Walk_START_Event : public DE::Event
{
	Player_Walk_START_Event()
		:DE::Event(GameEventID::Player_Walk_START_Event)
	{};
};

struct Player_Walk_END_Event : public DE::Event
{
	Player_Walk_END_Event()
		:DE::Event(GameEventID::Player_Walk_END_Event)
	{};
};

struct Player_Attack_1_START_Event : public DE::Event
{
	Player_Attack_1_START_Event()
		:DE::Event(GameEventID::Player_Attack_1_START_Event)
	{};
};

struct Player_Attack_1_END_Event : public DE::Event
{
	Player_Attack_1_END_Event()
		:DE::Event(GameEventID::Player_Attack_1_END_Event)
	{};
};

#endif // !GAME_EVENT_H
