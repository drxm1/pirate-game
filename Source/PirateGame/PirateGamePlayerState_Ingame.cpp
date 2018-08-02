// Copyright (C) Dominik Rosser 2018

#include "PirateGamePlayerState_Ingame.h"

APirateGamePlayerState_Ingame::APirateGamePlayerState_Ingame()
	: RemainingLifes (1u)
	, CollectedGoldCoins (0)
{
}

void APirateGamePlayerState_Ingame::BeginPlay()
{
	Super::BeginPlay();
}

void APirateGamePlayerState_Ingame::LoseLife()
{
	if (RemainingLifes > 0)
		--RemainingLifes;
}

void APirateGamePlayerState_Ingame::AddLife()
{
	if (/*RemainingLifes < UINT8_MAX && */
		RemainingLifes < 3 /* 3: Max Hearts Displayed in Widget */)
		++RemainingLifes;
}

bool APirateGamePlayerState_Ingame::IsGameOver()
{
	return RemainingLifes == 0;
}

void APirateGamePlayerState_Ingame::AddCollectedGoldCoin()
{
	if (CollectedGoldCoins != INT32_MAX)
		++CollectedGoldCoins;
}



