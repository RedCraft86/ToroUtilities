// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroCameraManager.h"

AToroCameraManager::AToroCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AToroCameraManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
