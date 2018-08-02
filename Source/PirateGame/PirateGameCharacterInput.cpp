// Copyright (C) Dominik Rosser 2018

#include "PirateGameCharacterInput.h"

FPirateGameCharacterInput::FPirateGameCharacterInput()
{
}

FPirateGameCharacterInput::~FPirateGameCharacterInput()
{
}

/** 
 * Validates and restricts the RawMovementInput 
 * and sets the MovementInput to the movement value to be handled.
 */
void FPirateGameCharacterInput::Sanitize()
{
	// Restrict the maximum movement values and add the value to the MovementInput to be handled by the character
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);

	/* Would add another effect in addition if we could move in multiple axis and want to normalize it
	   MovementInput = MovementInput.GetSafeNormal();*/

	// Prevent going faster and faster
	RawMovementInput.Set(0.0f, 0.0f);
}

/** 
 * Adds a value to the RawMovementInput to be handled via the Sanitize function.
 */
void FPirateGameCharacterInput::MoveX(float AxisValue)
{
	RawMovementInput.X += AxisValue;
}
