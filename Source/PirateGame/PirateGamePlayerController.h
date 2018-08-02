// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PirateGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APirateGamePlayerController();

	virtual void BeginPlay() override;
	
	
};
