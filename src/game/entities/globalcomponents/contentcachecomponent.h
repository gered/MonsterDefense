#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_CONTENTCACHECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_CONTENTCACHECOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class ContentCache;

class ContentCacheComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "ContentCacheComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ContentCacheComponent();
	void Reset();

	ContentCache *cache;
};

inline ContentCacheComponent::ContentCacheComponent()
{
	Reset();
}

inline void ContentCacheComponent::Reset()
{
	cache = NULL;
}

#endif

