// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bIsActive(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Exclude from HLOD assuming it is gameplay specific
	Identity.RegenerateGuid();

	SetCanBeDamaged(false);
}

void AToroActor::SetActiveState_Implementation(const bool bActive)
{
	if (bIsActive != bActive)
	{
		bIsActive = bActive;
		ApplyActiveState(bActive);
	}
}

void AToroActor::ApplyActiveState_Implementation(const bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	ApplyActiveState(bIsActive);
}

#if WITH_EDITOR
void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Identity.RegenerateGuid(true);
}

bool AToroActor::ShouldTickIfViewportsOnly() const
{
	return bTickInEditor;
}
#endif
