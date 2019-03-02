#include "../../framework/debug.h"

#include "overheadfollowcamera.h"

#include "../../entities/entity.h"
#include "../../game/entities/components/positioncomponent.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/math/mathhelpers.h"
#include "../../framework/math/vector3.h"

OverheadFollowCamera::OverheadFollowCamera(GraphicsDevice *graphicsDevice)
	: Camera(graphicsDevice)
{
	STACK_TRACE;
	m_entity = NULL;
	m_lastEntityPosition = ZERO_VECTOR;
	m_elevationAngle = DegreesToRadians(65);
	m_elevationDistance = 10.0f;
	
	m_facingAngle = 0.0f;
	m_isShiftingToNewDirection = FALSE;
	m_shiftProgress = 0.0f;
	m_startYAngle = 0.0f;
	m_endYAngle = 0.0f;
}

OverheadFollowCamera::~OverheadFollowCamera()
{
	STACK_TRACE;
}

void OverheadFollowCamera::SetFacingAngle(float angle)
{
	STACK_TRACE;
	ASSERT(m_isShiftingToNewDirection != TRUE);
	if (m_isShiftingToNewDirection)
		return;
	
	m_facingAngle = angle;
}

void OverheadFollowCamera::RotateBy(float radians)
{
	STACK_TRACE;
	if (m_isShiftingToNewDirection)
		return;
	
	m_isShiftingToNewDirection = TRUE;
	m_newFacingAngle = RolloverClamp(m_facingAngle + radians, RADIANS_0, RADIANS_360);
	m_shiftProgress = 0.0f;
	m_startYAngle = m_facingAngle;
	m_endYAngle = m_newFacingAngle;
	
	if (radians >= 0.0f)
	{
		if ((m_startYAngle + radians) >= RADIANS_360)
			m_startYAngle -= RADIANS_360;
		else if (IsCloseEnough(m_startYAngle + radians, RADIANS_360))
			m_startYAngle -= RADIANS_360;
		
		if (IsCloseEnough(m_endYAngle, RADIANS_360))
			m_endYAngle -= RADIANS_360;
	}
	else
	{
		// NOTE TO SELF: we're adding radians because it is negative already...
		//               herp-derp!
		if ((m_startYAngle + radians) <= RADIANS_0)
			m_startYAngle += RADIANS_360;
		else if (IsCloseEnough(m_startYAngle + radians, RADIANS_0))
			m_startYAngle -= RADIANS_360;
		
		if (IsCloseEnough(m_endYAngle, RADIANS_0))
			m_endYAngle += RADIANS_360;
	}
}

void OverheadFollowCamera::OnUpdate(float delta)
{
	STACK_TRACE;
	float yAngle = GetCurrentYAngle(delta);
	UpdatePosition(yAngle);
	CalculateDefaultLookAt(ZERO_VECTOR);
}

float OverheadFollowCamera::GetCurrentYAngle(float delta)
{
	STACK_TRACE;
	const float SHIFT_SPEED = 3.0f;
	
	float yAngle;
	if (m_isShiftingToNewDirection)
	{
		// continue shifting if we're already shifting
		yAngle = SmoothStep(m_startYAngle, m_endYAngle, m_shiftProgress);
		m_shiftProgress += (delta * SHIFT_SPEED);
		if (m_shiftProgress >= 1.0f)
		{
			// done shifting
			m_isShiftingToNewDirection = FALSE;
			m_facingAngle = m_newFacingAngle;
		}
	}
	else
		// not shifting, just use the current angle
		yAngle = m_facingAngle;
	
	return yAngle;
}

void OverheadFollowCamera::UpdatePosition(float yAngle)
{
	STACK_TRACE;
	if (m_entity != NULL)
	{
		PositionComponent *entityPosition = m_entity->Get<PositionComponent>();
		ASSERT(entityPosition != NULL);
		m_lastEntityPosition = entityPosition->position;
	}

	//const float Y_ANGLE = RADIANS_0;

	Vector3 position;
	// NOTE TO MY FUTURE SELF: the "RADIANS_90 - m_elevationAngle" below is essential
	// for calculating the correct x-angle orientation here and *IS TOTALLY
	// UNRELATED* to any of the camera/entity FacingDirection-to-angle
	// conversion crap (which should actually be obvious to you by simply 
	// taking the time to look at and read what is happening here)! 
	// don't touch this! it "just works" as-is :)
	GetCartesianCoordsFromSpherical(m_elevationDistance, RADIANS_90 - m_elevationAngle, yAngle, position.x, position.y, position.z);

	position += m_lastEntityPosition;

	GetPosition().Set(position.x, position.y, position.z);
	GetOrientation().Set(m_elevationAngle, RADIANS_360 - yAngle, 0.0f);
}

