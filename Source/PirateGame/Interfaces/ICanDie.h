// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanDie.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanDie : public UInterface
{
	GENERATED_BODY()
};

/**
 * The Actor implementing this interface must
 * (1) Store the value inside the SetHealth implementation anywhere
 * (2) Return the in SetHealth implementation stored value from the GetHealth implementation
 * (3) Check if the player is dead inside the SetHealth implementation and call OnDie() if he is
 */
class PIRATEGAME_API ICanDie
{
	GENERATED_BODY()

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** To be called when the Actor's health is zero or lower */
	virtual void OnDie() = 0;

	/** Returns the current health */
	virtual int GetHealth() const = 0;

	/** Should set the health to a new value. Should also check if the player is dead inside the implementation */
	virtual void SetHealth(const int value) = 0;

	/** Simply returns whether the actor is alive */
	bool IsAlive() const;

	/** Subtracts health using the SetHealth function which should then call the Die() function when the health falls under zero */
	void LoseHealth(const int value);
};
