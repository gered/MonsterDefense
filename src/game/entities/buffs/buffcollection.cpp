#include "../../../framework/debug.h"

#include "buffcollection.h"
#include "buff.h"

BuffCollection::BuffCollection(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

BuffCollection::~BuffCollection()
{
	STACK_TRACE;
	RemoveAll();
}

Buff* BuffCollection::Get(BUFF_TYPE type) const
{
	STACK_TRACE;
	BuffMap::const_iterator i = m_buffs.find(type);
	if (i == m_buffs.end())
		return NULL;
	else
		return i->second;
}

void BuffCollection::Remove(BUFF_TYPE type)
{
	STACK_TRACE;
	BuffMap::iterator i = m_buffs.find(type);
	ASSERT(i != m_buffs.end());
	if (i == m_buffs.end())
		return;
	
	Buff *buff = i->second;
	SAFE_DELETE(buff);
	
	m_buffs.erase(i);
}

void BuffCollection::RemoveAll()
{
	STACK_TRACE;
	while (!m_buffs.empty())
	{
		BuffMap::iterator i = m_buffs.begin();
		
		Buff *buff = i->second;
		SAFE_DELETE(buff);
		
		m_buffs.erase(i);
	}
}
