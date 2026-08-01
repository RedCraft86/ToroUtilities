// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bEnabled(true), bStartWithCollision(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Exclude this actor from HLOD assuming it is gameplay specific
	UniqueId.RegenerateGuid();

	SetCanBeDamaged(false);
}

void AToroActor::SetEnabledState_Implementation(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		EnableStateChanged(bEnabled);
		EnableStateChangedBP(bEnabled);
		OnEnableStateChanged.Broadcast(bEnabled);
		OnEnableStateChangedBP.Broadcast(bEnabled);
	}
}

void AToroActor::EnableStateChanged(const bool bState)
{
	SetActorHiddenInGame(!bState);
	SetActorEnableCollision(bStartWithCollision && bState);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bState);
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(bStartWithCollision);
	if (!bEnabled)
	{
		// Assumed enabled by default so will need to update states if disabled
		EnableStateChanged(bEnabled);
		EnableStateChangedBP(bEnabled);
	}
}

#if WITH_EDITOR
void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);
}
#endif