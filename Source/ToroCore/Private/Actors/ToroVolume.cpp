// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Actors/ToroVolume.h"

AToroVolume::AToroVolume(): bEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITOR
	DEFINE_ICON_COMPONENT
#endif

#if WITH_EDITORONLY_DATA
	bTickInEditor = false;
	bDisplayIcon = true;
	bColored = false;
	bShadedVolume = false;
	VolumeColor = FColor(124, 75, 155);
#endif

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);

	UniqueId.RegenerateGuid();
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
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bState);
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	if (!bEnabled) BroadcastStateChanged(); // Notify if disabled to change defaults which are assumed enabled
}

#if WITH_EDITOR
void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);

	bDisplayShadedVolume = bShadedVolume;
	bColored = bColoredVolume;
	BrushColor = VolumeColor;
	if (EdIconComp)
	{
		EdIconComp->SetVisibility(bDisplayIcon);
	}
}
#endif