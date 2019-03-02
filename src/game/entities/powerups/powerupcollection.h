#ifndef __GAME_ENTITIES_POWERUPS_POWERUPCOLLECTION_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_POWERUPCOLLECTION_H_INCLUDED__

#include "../../../framework/debug.h"

#include "powerup.h"
#include <stl/map.h>

class EntityManager;

typedef stl::map<POWERUP_TYPE, PowerUp*> PowerUpMap;

class PowerUpCollection
{
public:
	PowerUpCollection(EntityManager *entityManager);
	virtual ~PowerUpCollection();
	
	template<class T> T* Add();
	template<class T> T* Get() const;
	PowerUp* Get(POWERUP_TYPE type) const;
	template<class T> void Remove();
	void Remove(POWERUP_TYPE type);
	void RemoveAll();
	
private:
	PowerUpMap m_powerUps;
	EntityManager *m_entityManager;
};

template<class T>
T* PowerUpCollection::Add()
{
	STACK_TRACE;
	PowerUp *existing = Get(T::GetType());
	ASSERT(existing == NULL);
	if (existing != NULL)
		return NULL;
	
	T* newPowerUp = new T(m_entityManager);
	m_powerUps[T::GetType()] = newPowerUp;
	
	return newPowerUp;
}

template<class T>
T* PowerUpCollection::Get() const
{
	STACK_TRACE;
	PowerUp *p = Get(T::GetType());
	if (p == NULL)
		return NULL;
	else
		return p->As<T>();
}

template<class T>
void PowerUpCollection::Remove()
{
	STACK_TRACE;
	Remove(T::GetType());
}

#endif
