// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "ICharacterView.generated.h"

/**
 * General character interface to access character specific data from objects. (UObject Wrapper)
 */
UINTERFACE()
class UCharacterView : public UInterface
{
	GENERATED_BODY()
};

/**
 * General character interface to access character specific data from objects.
 */
class TOROCORE_API ICharacterView
{
	GENERATED_BODY()

public:

	/* The location (world-space) at which this character is looking at. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		bool GetLookPoint(FVector& Location) const;
	virtual bool GetLookPoint_Implementation(FVector& Location) const = 0;

	/* The location (world-space) that other characters will focus when looking at this character. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		FVector GetFocusPoint() const;
	virtual FVector GetFocusPoint_Implementation() const = 0;

	/* The location (world-space), forward vector, and field of view angle of this character's eyes. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		void GetViewPoint(FVector& Location, FVector& Forward, float& Angle) const;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const = 0;

	static bool GetLookPoint(const UObject* Target, FVector& Location)
	{
		if (Target && Target->Implements<UCharacterView>() 
			&& Execute_GetLookPoint(Target, Location))
		{
			return true;
		}

		Location = FVector::ZeroVector;
		return false;
	}

	static FVector GetFocusPoint(const UObject* Target)
	{
		if (!Target)
		{
			return FVector::ZeroVector;
		}

		if (Target->Implements<UCharacterView>())
		{
			return Execute_GetFocusPoint(Target);
		}
		if (const AActor* AsActor = Cast<AActor>(Target))
		{
			return AsActor->GetActorLocation();
		}
		if (const USceneComponent* AsComp = Cast<USceneComponent>(Target))
		{
			return AsComp->GetComponentLocation();
		}

		return FVector::ZeroVector;
	}

	static void GetViewPoint(const UObject* Target, FVector& Location, FVector& Forward, float& Angle)
	{
		if (!Target)
		{
			Location = FVector::ZeroVector;
			Forward = FVector::ZeroVector;
			Angle = 0.0f;
			return;
		}

		if (Target->Implements<UCharacterView>())
		{
			Execute_GetViewPoint(Target, Location, Forward, Angle);
			return;
		}

		Angle = 90.0f;
		if (const AActor* AsActor = Cast<AActor>(Target))
		{
			Location = AsActor->GetActorLocation();
			Forward = AsActor->GetActorForwardVector();
		}
		else if (const USceneComponent* AsComp = Cast<USceneComponent>(Target))
		{
			Location = AsComp->GetComponentLocation();
			Forward = AsComp->GetForwardVector();
		}
	}
};