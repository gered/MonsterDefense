#ifndef __GAME_EVENTS_QUITAPPEVENT_H_INCLUDED__
#define __GAME_EVENTS_QUITAPPEVENT_H_INCLUDED__

#include "../../framework/common.h"
#include "../../events/event.h"

struct QuitAppEvent : public Event
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "QuitAppEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
};

#endif
