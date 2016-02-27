#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include "Event.h"

namespace DE
{

	namespace EngineEventID
	{
		enum
		{
			Animation_END_Event = 0xFF,
		};
	}


	struct Animation_END_Event : public Event
	{
		Animation_END_Event()
			:Event(EngineEventID::Animation_END_Event)
		{};
	};

} // namespace DE

#endif //!ENGINE_EVENT_H
