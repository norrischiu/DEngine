#pragma once
#include "Event.h"

namespace AudioEventID
{
	enum
	{
		Audio_Play_Event,
		Audio_Pause_Event,
		Audio_Stop_Event
	};
}

namespace DE
{
	struct Audio_Event : public Event
	{
		Audio_Event(int audioEventId, const int event_id, const char* audio_id) :
			Event(AudioEventID::Audio_Play_Event), event_id(event_id), audio_id(audio_id)
		{

		}

	public:
		const char* audio_id;
		const int event_id;
	};

	struct Audio_Play_Event : public Audio_Event
	{
		Audio_Play_Event(const int event_id, const char* audio_id, const float volume, const float pitch, const float pan, const bool looping)
			: Audio_Event(AudioEventID::Audio_Play_Event, event_id, audio_id), volume(volume), pitch(pitch), pan(pan), looping(looping)
		{};

		public:
			const float volume;
			const float pitch;
			const float pan;
			const bool looping;
	};

	struct Audio_Pause_Event : public Audio_Event
	{
		Audio_Pause_Event(const int event_id, const char* audio_id)
			: Audio_Event(AudioEventID::Audio_Stop_Event, event_id, audio_id)
		{};
	};

	struct Audio_Stop_Event : public Audio_Event
	{
		Audio_Stop_Event(const int event_id, const char* audio_id)
			: Audio_Event(AudioEventID::Audio_Stop_Event, event_id, audio_id)
		{};
	};
}

