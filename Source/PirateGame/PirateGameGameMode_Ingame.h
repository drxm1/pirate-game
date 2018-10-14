// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PirateGameGameModeBase.h"
#include "PirateGameGameMode_Ingame.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGameGameMode_Ingame : public APirateGameGameModeBase
{
	GENERATED_BODY()
	
public:
	/** Shows the game over menu widget */
	UFUNCTION(BlueprintCallable, Category = "UMG Game Over Menu")
	void ShowGameOverMenuWidget();

	/** Shows the you won menu widget */
	UFUNCTION(BlueprintCallable, Category = "UMG Game Won Menu")
	void ShowGameWonMenuWidget();

	/** The widget class we will use when the game is over (lost). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game Over Menu")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	/** The widget class we will use when the game is over (won) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game Won Menu")
	TSubclassOf<UUserWidget> GameWonWidgetClass;

protected:
	/** The widget instance that is displayed when the game is lost */
	UPROPERTY()
	UUserWidget* CurrentGameOverMenuWidget;

	/** The widget instance that is displayed when the game is won */
	UPROPERTY()
	UUserWidget* CurrentGameWonMenuWidget;
};
