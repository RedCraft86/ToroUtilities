// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bEnabled(true), bStartWithCollision(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITOR
	DEFINE_VISUAL_COMPONENT
#endif

	SetCanBeDamaged(false);
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
	if (!bEnabled) BroadcastStateChanged();
}

void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!UniqueGuid.IsValid()) UniqueGuid = FGuid::NewGuid();
}
