#pragma once
#include "Event.h"

namespace AudioEventID
{
	enum
	{
		Audio_Play_Event,
		Audio_Stop_Event,
		Audio_Playing_Event
	};
}

namespace DE
{
	struct Audio_Play_Event : public Event
	{
		Audio_Play_Event(const char* audio_id, const int event_id, const float volume, const float pitch, const float pan, const bool looping)
			: Event(AudioEventID::Audio_Play_Event), audio_id(audio_id), event_id(event_id), volume(volume), pitch(pitch), pan(pan), looping(looping)
		{};

		public:
			const char* audio_id;
			const int event_id;
			const float volume;
			const float pitch;
			const float pan;
			const bool looping;
	};

	struct Audio_Stop_Event : public Event
	{
		Audio_Stop_Event()
			:Event(AudioEventID::Audio_Stop_Event)
		{};

		public:
			int audio_id;
	};
}

