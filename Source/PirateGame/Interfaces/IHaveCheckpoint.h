// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "IHaveCheckpoint.generated.h"

UINTERFACE(BlueprintType) class PIRATEGAME_API UHaveCheckpoint : public UInterface { GENERATED_BODY() };

/**
 * 
 */
class PIRATEGAME_API IHaveCheckpoint
{
	GENERATED_BODY()

public:
	// PLACE INTERFACE PROPERTIES HERE

	/** This function sets the checkpoint to the given location */
	void SetCheckpoint(const FVector & location) { }

	/** This function returns the current checkpoint of this entity */
	FVector GetCheckpoint() { return FVector(0.0f, 0.0f, 0.0f); }
};
