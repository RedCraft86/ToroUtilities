// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Actors/ToroVolume.h"

AToroVolume::AToroVolume(): bIsActive(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bEnableAutoLODGeneration = false; // Exclude from HLOD assuming it is gameplay specific
	Identity.RegenerateGuid();

	SetCanBeDamaged(false);
}

void AToroVolume::SetActiveState_Implementation(const bool bActive)
{
	if (bIsActive != bActive)
	{
		bIsActive = bActive;
		ApplyActiveState(bActive);
	}
}

void AToroVolume::ApplyActiveState_Implementation(const bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	ApplyActiveState(bIsActive);
}

#if WITH_EDITOR
void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Identity.RegenerateGuid(true);

	bColored = bColoredVolume;
	BrushColor = VolumeColor;
}

bool AToroVolume::ShouldTickIfViewportsOnly() const
{
	return bTickInEditor;
}
#endif
