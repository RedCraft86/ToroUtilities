// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "CharInterface.generated.h"

UINTERFACE()
class UCharInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API ICharInterface
{
	GENERATED_BODY()

public:

	/* (Hopefully) Unique Tag/ID of the character */
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		FGameplayTag GetCharacterID() const;
	virtual FGameplayTag GetCharacterID_Implementation() const { return {}; }

	/* The point that other characters focus on when looking at this character */
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		bool GetFocusPoint(FVector& Location) const;
	virtual bool GetFocusPoint_Implementation(FVector& Location) const { return false; }

	/* The point this character is currently focusing on */
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		bool GetViewTarget(FVector& Location) const;
	virtual bool GetViewTarget_Implementation(FVector& Location) const { return false; }

	/* The location, forward vector and angle in which the character's "eyes" are at */
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		void GetViewPoint(FVector& Location, FVector& Forward, float& Angle) const;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const {}

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) && Target->Implements<UCharInterface>(); 
	}

	static FGameplayTag GetCharacterID(const UObject* Target)
	{
		return ImplementedBy(Target) ? Execute_GetCharacterID(Target) : FGameplayTag::EmptyTag;
	}

	static FVector GetFocusPoint(const UObject* Target)
	{
		if (!Target)
		{
			return FVector::ZeroVector;
		}

		FVector Location = FVector::ZeroVector;
		if (!ImplementedBy(Target) || !Execute_GetFocusPoint(Target, Location))
		{
			if (const AActor* AsActor = Cast<AActor>(Target))
			{
				Location = AsActor->GetActorLocation();
			}
			else if (const USceneComponent* AsComponent = Cast<USceneComponent>(Target))
			{
				Location = AsComponent->GetComponentLocation();
			}
		}
		return Location;
	}

	static bool GetViewTarget(const UObject* Target, FVector& Location)
	{
		return ImplementedBy(Target) && Execute_GetViewTarget(Target, Location);
	}

	static void GetViewPoint(const UObject* Target, FVector& Location, FVector& Forward, float& FOV)
	{
		if (ImplementedBy(Target)) Execute_GetViewPoint(Target, Location, Forward, FOV);
	}
};