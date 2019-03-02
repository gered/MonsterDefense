#ifndef __GAME_ENTITIES_WEAPONS_WEAPONCOLLECTION_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_WEAPONCOLLECTION_H_INCLUDED__

#include "../../../framework/debug.h"

#include "weapon.h"
#include <stl/map.h>

class EntityManager;

typedef stl::map<WEAPON_TYPE, Weapon*> WeaponMap;

class WeaponCollection
{
public:
	WeaponCollection(EntityManager *entityManager);
	virtual ~WeaponCollection();
	
	template<class T> T* Add();
	template<class T> T* Get() const;
	Weapon* Get(WEAPON_TYPE type) const;
	template<class T> void Remove();
	void Remove(WEAPON_TYPE type);
	void RemoveAll();
	
private:
	WeaponMap m_weapons;
	EntityManager *m_entityManager;
};

template<class T>
T* WeaponCollection::Add()
{
	STACK_TRACE;
	Weapon *existing = Get(T::GetType());
	ASSERT(existing == NULL);
	if (existing != NULL)
		return NULL;
	
	T* newWeapon = new T(m_entityManager);
	m_weapons[T::GetType()] = newWeapon;
	
	return newWeapon;
}

template<class T>
T* WeaponCollection::Get() const
{
	STACK_TRACE;
	Weapon *w = Get(T::GetType());
	if (w == NULL)
		return NULL;
	else
		return w->As<T>();
}

template<class T>
void WeaponCollection::Remove()
{
	STACK_TRACE;
	Remove(T::GetType());
}

#endif
