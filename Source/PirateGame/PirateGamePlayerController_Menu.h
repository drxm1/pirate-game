// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PirateGamePlayerController.h"
#include "PirateGamePlayerController_Menu.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGamePlayerController_Menu : public APirateGamePlayerController
{
	GENERATED_BODY()
	
public:
	APirateGamePlayerController_Menu();
	
	virtual void BeginPlay() override;
};
