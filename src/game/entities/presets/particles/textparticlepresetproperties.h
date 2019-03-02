#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_TEXTPARTICLEPRESETPROPERTIES_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_TEXTPARTICLEPRESETPROPERTIES_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypresetargs.h"
#include "../../../../framework/graphics/color.h"
#include "../../../../framework/math/vector3.h"
#include "../../components/textcomponent.h"

struct TextParticlePresetProperties : public EntityPresetArgs
{
	static ENTITYPRESETARGS_TYPE GetType()
	{
		static ENTITYPRESETARGS_TYPE typeName = "TextParticlePresetProperties";
		return typeName;
	}
	ENTITYPRESETARGS_TYPE GetTypeOf() const                 { return GetType(); }
	
	TextParticlePresetProperties();
	
	char text[TEXTCOMPONENT_MAX_LENGTH];
	Vector3 position;
	BOOL useBillboardRendering;
	float billboardSizePerPixel;
	Color color;
	float lifeTime;
};

inline TextParticlePresetProperties::TextParticlePresetProperties()
{
	text[0] = '\0';
	position = ZERO_VECTOR;
	useBillboardRendering = FALSE;
	billboardSizePerPixel = 0.0f;
	color = COLOR_WHITE;
	lifeTime = 0.0f;
}

#endif
