#include "../../../framework/debug.h"

#include "powerupcollection.h"
#include "powerup.h"

PowerUpCollection::PowerUpCollection(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

PowerUpCollection::~PowerUpCollection()
{
	STACK_TRACE;
	RemoveAll();
}

PowerUp* PowerUpCollection::Get(POWERUP_TYPE type) const
{
	STACK_TRACE;
	PowerUpMap::const_iterator i = m_powerUps.find(type);
	if (i == m_powerUps.end())
		return NULL;
	else
		return i->second;
}

void PowerUpCollection::Remove(POWERUP_TYPE type)
{
	STACK_TRACE;
	PowerUpMap::iterator i = m_powerUps.find(type);
	ASSERT(i != m_powerUps.end());
	if (i == m_powerUps.end())
		return;
	
	PowerUp *powerUp = i->second;
	SAFE_DELETE(powerUp);
	
	m_powerUps.erase(i);
}

void PowerUpCollection::RemoveAll()
{
	STACK_TRACE;
	while (!m_powerUps.empty())
	{
		PowerUpMap::iterator i = m_powerUps.begin();
		
		PowerUp *powerUp = i->second;
		SAFE_DELETE(powerUp);
		
		m_powerUps.erase(i);
	}
}
