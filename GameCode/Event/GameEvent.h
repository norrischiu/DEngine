#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "DEngine\Event\Event.h"

namespace GameEventID
{
	enum
	{
		Player_Walk_PLAYING_Event = 0xFFF,
		Player_Walk_END_Event,
		Player_strafe_left_START_Event,
		Player_strafe_left_END_Event,
		Player_strafe_right_START_Event,
		Player_strafe_right_END_Event,
		Player_Attack_1_START_Event,
		Player_Attack_1_END_Event,
		Player_Attack_2_START_Event,
		Player_Attack_2_END_Event,
		Player_Attack_3_START_Event,
		Player_Attack_3_END_Event,
		Player_Charged_Attack_START_Event,
		Player_Charged_Attack_END_Event,
		Player_Dodge_START_Event,
		Player_Dodge_END_Event
	};
}


struct Player_Walk_PLAYING_Event : public DE::Event
{
	Player_Walk_PLAYING_Event()
		:DE::Event(GameEventID::Player_Walk_PLAYING_Event)
	{};

	// Movement direction, for non-pivot movement
	DE::Vector3 m_vDir;
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

struct Player_Attack_2_START_Event : public DE::Event
{
	Player_Attack_2_START_Event()
		:DE::Event(GameEventID::Player_Attack_2_START_Event)
	{};
};

struct Player_Attack_2_END_Event : public DE::Event
{
	Player_Attack_2_END_Event()
		:DE::Event(GameEventID::Player_Attack_2_END_Event)
	{};
};

struct Player_Attack_3_START_Event : public DE::Event
{
	Player_Attack_3_START_Event()
		:DE::Event(GameEventID::Player_Attack_3_START_Event)
	{};
};

struct Player_Attack_3_END_Event : public DE::Event
{
	Player_Attack_3_END_Event()
		:DE::Event(GameEventID::Player_Attack_3_END_Event)
	{};
};

struct Player_Charged_Attack_START_Event : public DE::Event
{
	Player_Charged_Attack_START_Event()
		:DE::Event(GameEventID::Player_Charged_Attack_START_Event)
	{};
};

struct Player_Charged_Attack_END_Event : public DE::Event
{
	Player_Charged_Attack_END_Event()
		:DE::Event(GameEventID::Player_Charged_Attack_END_Event)
	{};
};

struct Player_Dodge_START_Event : public DE::Event
{
	Player_Dodge_START_Event()
		:DE::Event(GameEventID::Player_Dodge_START_Event)
	{};

	// Dodge direction
	DE::Vector3 m_vDir;
};

struct Player_Dodge_END_Event : public DE::Event
{
	Player_Dodge_END_Event()
		:DE::Event(GameEventID::Player_Dodge_END_Event)
	{};
};

#endif // !GAME_EVENT_H
