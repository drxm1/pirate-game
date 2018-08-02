// Copyright (C) Dominik Rosser 2018.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "PirateGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
// Constructor
public:
	/** Contstucts a APirateGameGameModeBase object */
	APirateGameGameModeBase();
	
// AActor overrides
protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

// Menu widget change system
public:
	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Menu")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	/** Shows or hides the paused menu widget. This is on another layer than the menu widget, 
	    in case someone ever managed to pause while inside another menu. 
		This way the user will still be able to access the other menu after unpausing again. */
	UFUNCTION(BlueprintCallable, Category = "UMG Pause Game Menu")
	void ShowPauseMenuWidget(const bool bShow = true);

protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Menu")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget class we will use as our menu when the game is paused. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Pause Game")
	TSubclassOf<UUserWidget> GamePausedWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentMenuWidget;

	/** The widget instance that is displayed when the game is paused */
	UPROPERTY()
	UUserWidget* CurrentGamePausedMenuWidget;
};
