#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_CONTENTMANAGERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_CONTENTMANAGERCOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class ContentManager;

class ContentManagerComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "ContentManagerComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ContentManagerComponent();
	void Reset();

	ContentManager *content;
};

inline ContentManagerComponent::ContentManagerComponent()
{
	Reset();
}

inline void ContentManagerComponent::Reset()
{
	content = NULL;
}

#endif

