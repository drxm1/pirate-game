// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PirateGameCharacterInput.generated.h"

/**
* This class covers all possible input schemes.
* What the inputs do can vary by character, but the same inputs will always exist.
*/
USTRUCT(BlueprintType)
struct PIRATEGAME_API FPirateGameCharacterInput
{
	GENERATED_USTRUCT_BODY()

public:
	/** FPirateGameCharacterInput construction */
	FPirateGameCharacterInput();

	/** FPirateGameCharacterInput destruction */
	~FPirateGameCharacterInput();

public:
	/** Validates and restricts the RawMovementInput and sets the MovementInput to the movement value to be handled */
	void Sanitize();

	/** Adds a value to the RawMovementInput to be handled via the Sanitize function */
	void MoveX(float AxisValue);

public:
	/** Sanitized movement input, ready to be used by game logic */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Input")
	FVector2D MovementInput;

private:
	/** Raw, unhandled input data.
	    Private because it's internal, raw data. 
		Game code should never see this. */
	FVector2D RawMovementInput;
};
