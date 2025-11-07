// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroVolume.h"

AToroVolume::AToroVolume(): bEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetBrushComponent()->SetCollisionProfileName("Trigger");
	SetCanBeDamaged(false);

#if WITH_EDITOR
	DEFINE_VISUAL_COMPONENT
#endif
	
#if WITH_EDITOR
	bColored = true;
	BrushColor = FColor(124, 75, 155);
#endif
}

void AToroVolume::SetEnabledState_Implementation(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		BroadcastStateChanged();
	}
}

void AToroVolume::BroadcastStateChanged()
{
	EnableStateChanged(bEnabled);
	EnableStateChangedBP(bEnabled);
	OnEnableStateChanged.Broadcast(bEnabled);
	OnEnableStateChangedBP.Broadcast(bEnabled);
}

void AToroVolume::EnableStateChanged(const bool bState)
{
	SetActorHiddenInGame(!bState);
	SetActorEnableCollision(bState);
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	if (!bEnabled) BroadcastStateChanged();
}

void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!UniqueGuid.IsValid()) UniqueGuid = FGuid::NewGuid();
}
