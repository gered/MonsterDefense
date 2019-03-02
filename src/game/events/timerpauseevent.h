#ifndef __GAME_EVENTS_TIMERPAUSEEVENT_H_INCLUDED__
#define __GAME_EVENTS_TIMERPAUSEEVENT_H_INCLUDED__

#include "../../framework/common.h"
#include "../../events/event.h"

struct TimerPauseEvent : public Event
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "TimerPauseEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
};

#endif
