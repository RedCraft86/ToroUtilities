// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/SimpleCooldown.h"

void FSimpleCooldown::Reset()
{
	Cooldown = Interval;
}

void FSimpleCooldown::Ready()
{
	Cooldown = 0.0f;
}

bool FSimpleCooldown::Tick(const float DeltaTime)
{
	if ((Cooldown -= DeltaTime) <= 0.0f)
	{
		Cooldown = Interval;
		return true;
	}

	return false;
}

void USimpleCooldownLibrary::ResetCooldown(FSimpleCooldown& InCooldown)
{
	InCooldown.Reset();
}

void USimpleCooldownLibrary::ReadyCooldown(FSimpleCooldown& InCooldown)
{
	InCooldown.Ready();
}

bool USimpleCooldownLibrary::TickCooldown(FSimpleCooldown& InCooldown, const float DeltaTime)
{
	return InCooldown.Tick(DeltaTime);
}
