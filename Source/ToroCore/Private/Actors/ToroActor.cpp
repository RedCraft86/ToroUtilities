// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bEnabled(true), bStartWithCollision(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

#if WITH_EDITOR
	DEFINE_ICON_COMPONENT
#endif

	SetCanBeDamaged(false);

	UniqueId.RegenerateGuid();
}

void AToroActor::SetEnabledState_Implementation(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		BroadcastStateChanged();
	}
}

void AToroActor::BroadcastStateChanged()
{
	EnableStateChanged(bEnabled);
	EnableStateChangedBP(bEnabled);
	OnEnableStateChanged.Broadcast(bEnabled);
	OnEnableStateChangedBP.Broadcast(bEnabled);
}

void AToroActor::EnableStateChanged(const bool bState)
{
	SetActorHiddenInGame(!bState);
	SetActorEnableCollision(bState);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bState);
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(bStartWithCollision);
	if (!bEnabled) BroadcastStateChanged(); // Notify if disabled to change defaults which are assumed enabled
}

#if WITH_EDITOR
void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);
}
#endif
