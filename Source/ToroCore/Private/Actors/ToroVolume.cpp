// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Actors/ToroVolume.h"

AToroVolume::AToroVolume(): bEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITORONLY_DATA
	bTickInEditor = false;
	bDisplayIcon = true;
	bColored = false;
	bShadedVolume = false;
	VolumeColor = FColor(124, 75, 155);
#endif

	bEnableAutoLODGeneration = false; // Exclude this actor from HLOD assuming it is gameplay specific
	UniqueId.RegenerateGuid();

	SetCanBeDamaged(false);
}

void AToroVolume::SetEnabledState_Implementation(const bool bInEnabled)
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

void AToroVolume::EnableStateChanged(const bool bState)
{
	SetActorEnableCollision(bState);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bState);
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	if (!bEnabled)
	{
		// Assumed enabled by default so will need to update states if disabled
		EnableStateChanged(bEnabled);
		EnableStateChangedBP(bEnabled);
	}
}

#if WITH_EDITOR
void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);

	bDisplayShadedVolume = bShadedVolume;
	bColored = bColoredVolume;
	BrushColor = VolumeColor;
}
#endif