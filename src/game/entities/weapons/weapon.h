#ifndef __GAME_ENTITIES_WEAPONS_WEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_WEAPON_H_INCLUDED__

#include "../../../framework/common.h"

class Entity;
class EntityManager;

typedef const char* WEAPON_TYPE;

class Weapon
{
public:
	Weapon(EntityManager *entityManager);
	virtual ~Weapon();

	virtual void OnAttack(Entity *attacker);
	virtual BOOL IsMelee() const = 0;
	virtual float GetCooldownTime() const = 0;
	virtual uint32_t GetImageIndex() const = 0;
	
	virtual WEAPON_TYPE GetTypeOf() const = 0;
	
	template<class T> BOOL Is() const;
	BOOL Is(WEAPON_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
	
protected:
	EntityManager* GetEntityManager() const                { return m_entityManager; }
	
	float GetDamageBonusForAttacker(Entity *attacker) const;
	
private:
	EntityManager *m_entityManager;
};

template<class T>
inline BOOL Weapon::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Weapon::Is(WEAPON_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Weapon::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Weapon::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
