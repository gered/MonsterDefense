#ifndef __GAME_EVENTS_TIMERSETEVENT_H_INCLUDED__
#define __GAME_EVENTS_TIMERSETEVENT_H_INCLUDED__

#include "../../framework/common.h"
#include "../../events/event.h"

struct TimerSetEvent : public Event
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "TimerSetEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	TimerSetEvent();
	
	uint32_t milliseconds;
	BOOL isCountingDown;
};

inline TimerSetEvent::TimerSetEvent()
{
	milliseconds = 0;
	isCountingDown = FALSE;
}

#endif
