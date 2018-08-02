// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PirateGamePlayerState_Base.h"
#include "PirateGamePlayerState_Ingame.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGamePlayerState_Ingame : public APirateGamePlayerState_Base
{
	GENERATED_BODY()
	
public:
	APirateGamePlayerState_Ingame();

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	uint8 RemainingLifes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	int32 CollectedGoldCoins;

public:
	UFUNCTION(BlueprintCallable)
	void LoseLife();

	UFUNCTION(BlueprintCallable)
	void AddLife();

	UFUNCTION(BlueprintCallable)
	bool IsGameOver();

	UFUNCTION(BlueprintCallable)
	void AddCollectedGoldCoin();
};
