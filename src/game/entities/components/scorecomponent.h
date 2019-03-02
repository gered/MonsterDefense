#ifndef __GAME_ENTITIES_COMPONENTS_SCORECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_SCORECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class ScoreComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ScoreComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	ScoreComponent();
	void Reset();
	
	int32_t score;
};

inline ScoreComponent::ScoreComponent()
{
	Reset();
}

inline void ScoreComponent::Reset()
{
	score = 0;
}

#endif

