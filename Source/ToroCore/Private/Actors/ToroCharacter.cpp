// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Actors/ToroCharacter.h"
#include "Components/CapsuleComponent.h"

AToroCharacter::AToroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	BaseEyeHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
	bEnableAutoLODGeneration = false; // Exclude this actor from HLOD assuming it is gameplay specific
	UniqueId.RegenerateGuid();

	SetCanBeDamaged(false);
}

bool AToroCharacter::SetControlRotation(const FRotator& Rotation, const bool bApplyRoll) const
{
	if (AController* CharController = GetController())
	{
		CharController->SetControlRotation(FRotator(Rotation.Pitch, Rotation.Yaw, bApplyRoll ? Rotation.Roll : 0.0f));
		return true;
	}
	return false;
}

void AToroCharacter::Teleport(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocation(Location, false, nullptr, ETeleportType::ResetPhysics);
	if (!SetControlRotation(Rotation, false))
	{
		SetActorRotation(FRotator(Rotation.Pitch, Rotation.Yaw, 0.0f), ETeleportType::ResetPhysics);
	}
}

bool AToroCharacter::GetLookTarget_Implementation(FVector& Location) const
{
	Location = FVector::ZeroVector;
	return false;
}

FVector AToroCharacter::GetFocusPoint_Implementation() const
{
	return GetPawnViewLocation();
}

#if WITH_EDITOR
void AToroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);
}
#endif