#ifndef __GAME_ENTITIES_BUFFS_BUFFCOLLECTION_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_BUFFCOLLECTION_H_INCLUDED__

#include "../../../framework/debug.h"

#include "buff.h"
#include <stl/map.h>

class EntityManager;

typedef stl::map<BUFF_TYPE, Buff*> BuffMap;

class BuffCollection
{
public:
	BuffCollection(EntityManager *entityManager);
	virtual ~BuffCollection();
	
	template<class T> T* Add();
	template<class T> T* Get() const;
	Buff* Get(BUFF_TYPE type) const;
	template<class T> void Remove();
	void Remove(BUFF_TYPE type);
	void RemoveAll();
	
private:
	BuffMap m_buffs;
	EntityManager *m_entityManager;
};

template<class T>
T* BuffCollection::Add()
{
	STACK_TRACE;
	Buff *existing = Get(T::GetType());
	ASSERT(existing == NULL);
	if (existing != NULL)
		return NULL;
	
	T* newBuff = new T(m_entityManager);
	m_buffs[T::GetType()] = newBuff;
	
	return newBuff;
}

template<class T>
T* BuffCollection::Get() const
{
	STACK_TRACE;
	Buff *b = Get(T::GetType());
	if (b == NULL)
		return NULL;
	else
		return b->As<T>();
}

template<class T>
void BuffCollection::Remove()
{
	STACK_TRACE;
	Remove(T::GetType());
}

#endif
