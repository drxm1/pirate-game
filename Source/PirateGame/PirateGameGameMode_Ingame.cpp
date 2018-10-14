// Copyright (C) Dominik Rosser 2018

#include "PirateGameGameMode_Ingame.h"

/** 
 * Shows the game over menu widget
 */
void APirateGameGameMode_Ingame::ShowGameOverMenuWidget()
{
	// Hide existing widget
	if (CurrentGameOverMenuWidget != nullptr)
	{
		CurrentGameOverMenuWidget->RemoveFromViewport();
		CurrentGameOverMenuWidget = nullptr;
	}
	// Definetly hide the other widget (A player can't lose and win at the same time
	if (CurrentGameWonMenuWidget != nullptr)
	{
		CurrentGameWonMenuWidget->RemoveFromViewport();
		CurrentGameWonMenuWidget = nullptr;
	}
	// Create and display the widget
	CurrentGameOverMenuWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (CurrentGameOverMenuWidget != nullptr)
	{
		CurrentGameOverMenuWidget->AddToViewport();
	}
	// Show mouse cursor
	APlayerController* const pC = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (pC != nullptr)
	{
		pC->bShowMouseCursor = true;
	}
}

/**
 * Shows the game won menu widget
 */
void APirateGameGameMode_Ingame::ShowGameWonMenuWidget()
{
	// Hide existing widget
	if (CurrentGameWonMenuWidget != nullptr)
	{
		CurrentGameWonMenuWidget->RemoveFromViewport();
		CurrentGameWonMenuWidget = nullptr;
	}
	// Definetly hide the other widget (A player can't lose and win at the same time
	if (CurrentGameOverMenuWidget != nullptr)
	{
		CurrentGameOverMenuWidget->RemoveFromViewport();
		CurrentGameOverMenuWidget = nullptr;
	}
	// Create and display the widget
	CurrentGameWonMenuWidget = CreateWidget<UUserWidget>(GetWorld(), GameWonWidgetClass);
	if (CurrentGameWonMenuWidget != nullptr)
	{
		CurrentGameWonMenuWidget->AddToViewport();
	}
	// Show mouse cursor
	APlayerController *const pC = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (pC != nullptr)
	{
		pC->bShowMouseCursor = true;
	}
}