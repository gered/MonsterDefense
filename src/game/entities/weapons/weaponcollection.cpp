#include "../../../framework/debug.h"

#include "weaponcollection.h"
#include "weapon.h"

WeaponCollection::WeaponCollection(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

WeaponCollection::~WeaponCollection()
{
	STACK_TRACE;
	RemoveAll();
}

Weapon* WeaponCollection::Get(WEAPON_TYPE type) const
{
	STACK_TRACE;
	WeaponMap::const_iterator i = m_weapons.find(type);
	if (i == m_weapons.end())
		return NULL;
	else
		return i->second;
}

void WeaponCollection::Remove(WEAPON_TYPE type)
{
	STACK_TRACE;
	WeaponMap::iterator i = m_weapons.find(type);
	ASSERT(i != m_weapons.end());
	if (i == m_weapons.end())
		return;
	
	Weapon *weapon = i->second;
	SAFE_DELETE(weapon);
	
	m_weapons.erase(i);
}

void WeaponCollection::RemoveAll()
{
	STACK_TRACE;
	while (!m_weapons.empty())
	{
		WeaponMap::iterator i = m_weapons.begin();
		
		Weapon *weapon = i->second;
		SAFE_DELETE(weapon);
		
		m_weapons.erase(i);
	}
}
