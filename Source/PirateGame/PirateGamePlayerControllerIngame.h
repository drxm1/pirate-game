// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PirateGamePlayerController.h"
#include "PirateGamePlayerControllerIngame.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGamePlayerControllerIngame : public APirateGamePlayerController
{
	GENERATED_BODY()
	
public:
	APirateGamePlayerControllerIngame();
	
	virtual void BeginPlay() override;

protected:
	/** Defines how many lifes are left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character lifes")
	uint8 RemainingLifes;
};
