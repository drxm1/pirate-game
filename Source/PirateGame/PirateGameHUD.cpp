// Copyright (C) Dominik Rosser 2018

#include "PirateGameHUD.h"

void APirateGameHUD::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UUserWidget> &widgetClass : DisplayWidgetClasses)
	{
		UUserWidget * newWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (newWidget != nullptr)
		{
			newWidget->AddToViewport();
			DisplayingWidgets.Add(newWidget);
		}
	}
}




