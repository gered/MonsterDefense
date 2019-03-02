#ifndef __GAME_ENTITIES_COMPONENTS_AUTOANIMATIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AUTOANIMATIONCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../entitystates.h"
#include <stl/map.h>
#include <stl/string.h>
#include <stl/utility.h>

struct AutoAnimationSequenceProperties
{
	BOOL loop;
	BOOL cannotBeOverridden;
};

typedef stl::pair<stl::string, AutoAnimationSequenceProperties> EntityStateSequenceProps;
typedef stl::map<ENTITYSTATES, EntityStateSequenceProps> SequenceToEntityStateMap;

class AutoAnimationComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AutoAnimationComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	AutoAnimationComponent();
	void Reset();

	AutoAnimationComponent* Set(ENTITYSTATES state, const stl::string &sequenceName, BOOL loop = TRUE, BOOL cannotBeOverridden = FALSE);
	const EntityStateSequenceProps* Get(ENTITYSTATES state);
	void Remove(ENTITYSTATES state);

private:
	SequenceToEntityStateMap sequenceNames;
};

inline AutoAnimationComponent::AutoAnimationComponent()
{
	Reset();
}

inline void AutoAnimationComponent::Reset()
{
	sequenceNames.clear();
}

inline AutoAnimationComponent* AutoAnimationComponent::Set(ENTITYSTATES state, const stl::string &sequenceName, BOOL loop, BOOL cannotBeOverridden)
{
	EntityStateSequenceProps p;
	p.first = sequenceName;
	p.second.loop = loop;
	p.second.cannotBeOverridden = cannotBeOverridden;

	sequenceNames[state] = p;

	return this;
}

inline const EntityStateSequenceProps* AutoAnimationComponent::Get(ENTITYSTATES state)
{
	SequenceToEntityStateMap::const_iterator i = sequenceNames.find(state);
	if (i == sequenceNames.end())
		return NULL;
	else
	{
		const EntityStateSequenceProps *props = &i->second;
		return props;
	}
}

inline void AutoAnimationComponent::Remove(ENTITYSTATES state)
{
	sequenceNames.erase(state);
}

#endif

