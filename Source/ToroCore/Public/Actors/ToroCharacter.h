// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/IIdentifiable.h"
#include "ToroCharacter.generated.h"

/**
 * A base Character class for the ToroUtilities framework with an identity.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroCharacter : public ACharacter, public IIdentifiable
{
	GENERATED_BODY()

public:

	AToroCharacter();

	/**
	 * Sets the rotation of the Controller (the 'view' rotation if on a player).
	 * @return True if rotation was successfully applied to a valid controller.
	 */
	UFUNCTION(BlueprintCallable, Category = Pawn)
		bool SetControlRotation(const FRotator& Rotation, const bool bApplyRoll = false) const;

	/**
	 * Teleports the character and sets the rotation. Roll rotation is discarded.
	 */
	UFUNCTION(BlueprintCallable, Category = Character)
		virtual void Teleport(const FVector& Location, const FRotator& Rotation);

	/**
	 * Gets the location (world-space) at which this character should be looking at. (if available)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		bool GetLookTarget(FVector& Location) const;
	bool GetLookTarget_Implementation(FVector& Location) const;

	/**
	 * Gets the location (world-space) at which other characters will look at when looking at this character.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		FVector GetFocusPoint() const;
	FVector GetFocusPoint_Implementation() const;

	virtual FGameObjectId GetIdentity_Implementation() const override { return UniqueId; }

protected:

	/**
	 * Identifier for saves system and other systems that involves identification.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

#if WITH_EDITORONLY_DATA
	/**
	 * If true, the Tick function will run inside the Level Editor viewport.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	virtual void PostInitializeComponents() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
