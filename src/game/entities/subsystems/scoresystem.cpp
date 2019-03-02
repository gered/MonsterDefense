#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "scoresystem.h"
#include "../entitystates.h"
#include "../components/positioncomponent.h"
#include "../components/scorecomponent.h"
#include "../presets/particles/textparticlepreset.h"
#include "../presets/particles/textparticlepresetproperties.h"
#include "../events/scoreevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include <string.h>

ScoreSystem::ScoreSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<ScoreEvent>();
}

ScoreSystem::~ScoreSystem()
{
	STACK_TRACE;
	StopListeningFor<ScoreEvent>();
}

void ScoreSystem::OnUpdate(float delta)
{
	STACK_TRACE;
}

BOOL ScoreSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<ScoreEvent>())
	{
		const ScoreEvent *e = event->As<ScoreEvent>();
		ScoreComponent *entityScore = e->GetEntity()->Get<ScoreComponent>();
		if (entityScore != NULL)
		{
			entityScore->score += e->scoreAwarded;
			
			Vector3 position;
			if (e->hasSourcePosition)
				position = e->sourcePosition;
			else
			{
				PositionComponent *scoreEntityPosition = e->GetEntity()->Get<PositionComponent>();
				ASSERT(scoreEntityPosition != NULL);
				position = scoreEntityPosition->position;
			}
			
			TextParticlePresetProperties textProps;
			textProps.position = position;
			textProps.useBillboardRendering = FALSE;
			textProps.color = COLOR_WHITE;
			textProps.lifeTime = 1.0f;
			snprintf(textProps.text, TEXTCOMPONENT_MAX_LENGTH, "%d", e->scoreAwarded);
			
			GetEntityManager()->AddUsingPreset<TextParticlePreset>(&textProps);
		}
	}
	
	return FALSE;
}
