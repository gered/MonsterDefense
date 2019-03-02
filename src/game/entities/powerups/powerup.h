#ifndef __GAME_ENTITIES_POWERUPS_POWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_POWERUP_H_INCLUDED__

#include "../../../framework/common.h"

class Entity;
class EntityManager;

typedef const char* POWERUP_TYPE;

class PowerUp
{
public:
	PowerUp(EntityManager *entityManager);
	virtual ~PowerUp();
	
	virtual void OnEntityCreation(Entity *powerUpEntity);
	virtual void OnPickup(Entity *powerUpEntity, Entity *collector);
	virtual BOOL ShouldCreatedCollectedEntityOnPickup() const;
	
	virtual POWERUP_TYPE GetTypeOf() const = 0;
	
	template<class T> BOOL Is() const;
	BOOL Is(POWERUP_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
	
protected:
	void AwardScoreToEntity(Entity *entity, Entity *powerUpEntity, int32_t score);
	void HealEntity(Entity *entity, Entity *powerUpEntity, uint32_t amount);
	void HealEntity(Entity *entity, Entity *powerUpEntity, float percentAmount);
	void HurtEntity(Entity *entity, Entity *powerUpEntity, uint32_t amount);
	void HurtEntity(Entity *entity, Entity *powerUpEntity, float percentAmount);
	
	EntityManager* GetEntityManager() const                { return m_entityManager; }
	
private:
	EntityManager *m_entityManager;
};

template<class T>
inline BOOL PowerUp::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL PowerUp::Is(POWERUP_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* PowerUp::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* PowerUp::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
