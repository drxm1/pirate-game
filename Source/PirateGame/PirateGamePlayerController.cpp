// Fill out your copyright notice in the Description page of Project Settings.

#include "PirateGamePlayerController.h"

APirateGamePlayerController::APirateGamePlayerController()
{
}

void APirateGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
}

