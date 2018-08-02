// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "PirateGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TArray<TSubclassOf<UUserWidget>> DisplayWidgetClasses;

	UPROPERTY()
	TArray<UUserWidget*> DisplayingWidgets;

protected:
	void BeginPlay() override;
	
	
};
