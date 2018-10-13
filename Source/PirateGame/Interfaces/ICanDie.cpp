// Copyright (C) Dominik Rosser 2018

#include "ICanDie.h"


// Add default functionality here for any ICanDie functions that are not pure virtual.


bool ICanDie::IsAlive() const
{
	return this->GetHealth() > 0;
}

void ICanDie::LoseHealth(const int value)
{
	SetHealth(GetHealth() - value);
}