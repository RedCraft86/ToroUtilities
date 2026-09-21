// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Actors/ToroCharacter.h"
#include "Components/CapsuleComponent.h"

AToroCharacter::AToroCharacter(): bIsActive(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bEnableAutoLODGeneration = false; // Exclude from HLOD assuming it is gameplay specific
	Identity.RegenerateGuid();

	SetCanBeDamaged(false);
}

void AToroCharacter::SetControlRotation(const FRotator& Rotation, const bool bIgnoreRoll) const
{
	FRotator TargetRotation = Rotation;
	if (bIgnoreRoll)
	{
		TargetRotation.Roll = GetControlRotation().Roll;
	}

	if (AController* Cont = GetController())
	{
		Cont->SetControlRotation(TargetRotation);
	}
}

bool AToroCharacter::GetFocusTarget_Implementation(FVector& Location) const
{
	Location = FVector::ZeroVector;
	return false;
}

FVector AToroCharacter::GetFaceLocation_Implementation() const
{
	return GetActorLocation() + GetActorUpVector() * (GetActorScale3D().Z * BaseEyeHeight);
}

void AToroCharacter::SetActiveState_Implementation(const bool bActive)
{
	if (bIsActive != bActive)
	{
		bIsActive = bActive;
		ApplyActiveState(bActive);
	}
}

bool AToroCharacter::TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{
	if (Super::TeleportTo(DestLocation, DestRotation, bIsATest, bNoCheck))
	{
		if (!bIsATest && HasActorBegunPlay())
		{
			SetControlRotation(DestRotation);
		}

		return true;
	}

	return false;
}

void AToroCharacter::ApplyActiveState_Implementation(const bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorTickEnabled(bActive);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, bActive ? ECR_Overlap : ECR_Ignore);
}

void AToroCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyActiveState(bIsActive);
}

void AToroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BaseEyeHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
}

#if WITH_EDITOR
void AToroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Identity.RegenerateGuid(true);
}

bool AToroCharacter::ShouldTickIfViewportsOnly() const
{
	return bTickInEditor;
}
#endif