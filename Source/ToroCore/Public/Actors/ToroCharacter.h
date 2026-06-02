// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/ICharacterView.h"
#include "Interfaces/IObjectIdentity.h"
#include "ToroCharacter.generated.h"

/**
 * Base character class implementing IObjectIdentity and specialized view interfaces.
 * Features:
 * - Persistent Unique Identity (GUID).
 * - Unified Toggle logic (Enable/Disable).
 * - Wrappers for common character operations like teleportation and view-point retrieval.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroCharacter : public ACharacter, public IObjectIdentity, public ICharacterView
{
	GENERATED_BODY()

public:

	AToroCharacter();

	/** 
	 * Sets the rotation of the Controller (the 'view' rotation).
	 * @return True if rotation was successfully applied to a valid controller.
	 */
	UFUNCTION(BlueprintCallable, Category = Pawn)
		bool SetControlRotation(const FRotator& Rotation, const bool bIncludeRoll = false) const;

	/** 
	 * Teleports the character while ensuring the view rotation is updated.
	 * Roll rotation is discarded for standard upright characters. 
	 */
	UFUNCTION(BlueprintCallable, Category = Character)
		virtual void Teleport(const FVector& Location, const FRotator& Rotation);

	virtual FGameObjectId GetGameObjectId_Implementation() const override { return UniqueId; }
	virtual bool GetLookPoint_Implementation(FVector& Location) const override { return false; }
	virtual FVector GetFocusPoint_Implementation() const override { return GetPawnViewLocation(); }
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;

protected:

	/** 
	 * Unique Identifier for save systems and cross-actor referencing.
	 * Marked as Transient for duplication/export to ensure IDs remain unique.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
