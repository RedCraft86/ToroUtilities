// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UObject/Interface.h"
#include "IPawnEntity.generated.h"

/**
 * General use interface to add general functions to pawns and characters. (UObject Wrapper)
 */
UINTERFACE()
class UPawnEntity : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to add general functions to pawns and characters.
 */
class TOROCORE_API IPawnEntity
{
	GENERATED_BODY()

public:

	/**
	 * Teleports this entity to the desired location and rotation.
	 * @param Location Location to teleport to.
	 * @param Rotation Rotation to turn to.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PawnEntity)
		void Teleport(const FVector& Location, const FRotator& Rotation);

	/**
	 * Sets the rotation of the Controller (the 'view' rotation if on a player).
	 * @param Rotation Rotation to apply to the controller.
	 * @param bApplyRoll Whether the Roll component should be zeroed. Off by default since pawns typically don't roll.
	 * @return True if rotation was successfully applied to a valid controller.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PawnEntity)
		bool SetControlRotation(const FRotator& Rotation, const bool bApplyRoll = false);

	/**
	 * Gets the location at which this entity should be looking at.
	 * @param Location World-Space location of the look target if one exist.
	 * @return True if a look target exist, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PawnEntity)
		bool GetLookTarget(FVector& Location) const;

	/**
	 * Gets the location at which other entities will look at when looking at this entity.
	 * @return World-Space location of this entity's "face" at which other entities will look at.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PawnEntity)
		FVector GetFocusPoint() const;

	static bool ImplementedBy(const UObject* Target)
	{
		return Target && Target->Implements<UPawnEntity>();
	}

	static void Teleport(UObject* Target, const FVector& Location, const FRotator& Rotation)
	{
		if (ImplementedBy(Target))
		{
			Execute_Teleport(Target, Location, Rotation);
		}
	}

	static bool SetControlRotation(UObject* Target, const FRotator& Rotation, const bool bApplyRoll = false)
	{
		return ImplementedBy(Target) && Execute_SetControlRotation(Target, Rotation, bApplyRoll);
	}

	static bool Teleport(const UObject* Target, FVector& Location)
	{
		return ImplementedBy(Target) && Execute_GetLookTarget(Target, Location);
	}

	static FVector Teleport(const UObject* Target)
	{
		return ImplementedBy(Target) ? Execute_GetFocusPoint(Target) : FVector::ZeroVector;
	}
};
