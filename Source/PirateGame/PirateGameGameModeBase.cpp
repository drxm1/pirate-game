// Fill out your copyright notice in the Description page of Project Settings.

#include "PirateGameGameModeBase.h"
#include "Engine.h"

/** 
 * Contstucts a APirateGameGameModeBase object.
 */
APirateGameGameModeBase::APirateGameGameModeBase()
{

}

/** 
 * Called when the game starts. 
 */
void APirateGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

/** 
 * Remove the current menu widget and create a new one 
 * from the specified class, if provided.
 */
void APirateGameGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentMenuWidget != nullptr)
	{
		CurrentMenuWidget->RemoveFromViewport();
		CurrentMenuWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentMenuWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentMenuWidget != nullptr)
		{
			CurrentMenuWidget->AddToViewport();
		}
	}
}

/** 
 * Shows or hides the paused menu widget. 
 * This is on another layer than the menu widget,
 * in case someone ever managed to pause while inside another menu.
 * This way the user will still be able to access the other menu after unpausing again.
 */
void APirateGameGameModeBase::ShowPauseMenuWidget(const bool bShow /*= true */)
{
	// Hide or show Widget
	if (CurrentGamePausedMenuWidget != nullptr)
	{
		CurrentGamePausedMenuWidget->RemoveFromViewport();
		CurrentGamePausedMenuWidget = nullptr;
	}
	if (bShow)
	{
		CurrentGamePausedMenuWidget = CreateWidget<UUserWidget>(GetWorld(), GamePausedWidgetClass);
		if (CurrentGamePausedMenuWidget != nullptr)
		{
			CurrentGamePausedMenuWidget->AddToViewport();
		}
	}
	// Hide or show mouse cursor
	APlayerController* const pC = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (pC != nullptr)
	{
		pC->bShowMouseCursor = bShow;
	}
}
