#ifndef __ENTITIES_ENTITY_H_INCLUDED__
#define __ENTITIES_ENTITY_H_INCLUDED__

#include "../framework/common.h"

#include "entitymanager.h"
#include "entitypreset.h"

class Entity
{
public:
	Entity(EntityManager *entityManager);
	virtual ~Entity();

	template<class T> T* Get() const;
	template<class T> T* Add();
	template<class T> void Remove();
	template<class T> BOOL Has() const;
	
	template<class T> BOOL WasCreatedUsingPreset() const;
	BOOL WasCreatedUsingPreset(ENTITYPRESET_TYPE type) const;

protected:
	EntityManager *m_entityManager;
};

template<class T>
inline T* Entity::Get() const
{
	return m_entityManager->GetComponent<T>(this);
}

template<class T>
inline T* Entity::Add()
{
	return m_entityManager->AddComponent<T>(this);
}

template<class T>
inline void Entity::Remove()
{
	m_entityManager->RemoveComponent<T>(this);
}

template<class T>
inline BOOL Entity::Has() const
{
	return m_entityManager->HasComponent<T>(this);
}

template<class T>
inline BOOL Entity::WasCreatedUsingPreset() const
{
	return m_entityManager->WasCreatedUsingPreset<T>(this);
}

inline BOOL Entity::WasCreatedUsingPreset(ENTITYPRESET_TYPE type) const
{
	return m_entityManager->WasCreatedUsingPreset(this, type);
}

#endif

