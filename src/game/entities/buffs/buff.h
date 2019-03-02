#ifndef __GAME_ENTITIES_BUFFS_BUFF_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_BUFF_H_INCLUDED__

#include "../../../framework/common.h"

class Entity;
class EntityManager;

typedef const char* BUFF_TYPE;

class Buff
{
public:
	Buff(EntityManager *entityManager);
	virtual ~Buff();
	
	virtual void OnApply(Entity *entity);
	virtual void OnRemove(Entity *entity);
	
	virtual float GetDuration() const = 0;
	virtual uint32_t GetImageIndex() const = 0;
	
	virtual BUFF_TYPE GetTypeOf() const = 0;
	
	template<class T> BOOL Is() const;
	BOOL Is(BUFF_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

protected:
	EntityManager* GetEntityManager() const                { return m_entityManager; }
	
private:
	EntityManager *m_entityManager;
};

template<class T>
inline BOOL Buff::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Buff::Is(BUFF_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Buff::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Buff::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
