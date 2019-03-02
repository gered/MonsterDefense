#ifndef __GAME_ENTITIES_EVENTS_ENTITYEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ENTITYEVENT_H_INCLUDED__

#include "../../../events/event.h"

class Entity;

struct EntityEvent : public Event
{
	Entity* GetEntity() const                              { return m_entity; }

protected:
	EntityEvent(Entity *entity);

private:
	Entity *m_entity;
};

inline EntityEvent::EntityEvent(Entity *entity)
{
	m_entity = entity;
}

#endif

