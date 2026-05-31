// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroPlayerCameraManager.h"

AToroPlayerCameraManager::AToroPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AToroPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Fixes motion blur issues when paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Allows photomode-like systems to move the camera even when game is paused
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
