// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"

namespace CharacterTags
{
	DEFINE_GAMEPLAY_TAG(Character)
	DEFINE_GAMEPLAY_TAG_CHILD(Character, Player)
}

AToroCharacter* AToroCharacter::FindCharacter(const UObject* ContextObject, const TSubclassOf<AToroCharacter> Class, const FGameplayTag CharID)
{
	if (!ContextObject || !Class || !CharacterTags::IsValidTag(CharID)) return nullptr;
	const UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	for (AToroCharacter* Character : TActorRange(World, Class))
	{
		if (ICharInterface::GetCharacterID(Character) == CharID)
		{
			return Character;
		}
	}
	return nullptr;
}

void AToroCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}

void AToroCharacter::ChangeCharacterID(const FGameplayTag InNewID)
{
	if (CharacterTags::IsValidTag(InNewID) && CharacterID != InNewID)
	{
		CharacterID = InNewID;
	}
}

bool AToroCharacter::GetFocusPoint_Implementation(FVector& Location) const
{
	const float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
	Location = GetActorLocation() + FVector(0.0f, 0.0f, Height);
	return true;
}

bool AToroCharacter::GetViewTarget_Implementation(FVector& Location) const
{
	Location = FVector::ZeroVector;
	return false;
}

void AToroCharacter::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = GetPawnViewLocation();
	Forward = GetViewRotation().Vector();
	Angle = 60.0f;
}

void AToroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!UniqueGuid.IsValid()) UniqueGuid = FGuid::NewGuid();
}
