// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/ToroActorInterface.h"
#include "ToroCharacter.generated.h"

/**
 * Base character with a persistent identity and overridable activation behavior.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TORORUNTIME_API AToroCharacter : public ACharacter, public IToroActorInterface
{
	GENERATED_BODY()

public:

	AToroCharacter();

	/**
	 * Sets the controller rotation when this character has a controller.
	 * @param Rotation Desired control rotation.
	 * @param bIgnoreRoll Whether to preserve the current control rotation's roll.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Character)
		void SetControlRotation(const FRotator& Rotation, const bool bIgnoreRoll = true) const;

	/**
	 * Gets the world-space location this character should focus on.
	 * @param Location Receives the focus location when one is available.
	 * @return Whether a focus target is available.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure = false, Category = Character)
		bool GetFocusTarget(FVector& Location) const;
	bool GetFocusTarget_Implementation(FVector& Location) const;

	/**
	 * Gets the world-space location other characters should focus on when looking at this character.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure = false, Category = Character)
		FVector GetFaceLocation() const;
	FVector GetFaceLocation_Implementation() const;

	virtual FGameObjectId GetIdentity_Implementation() const override { return Identity; }
	virtual bool GetActiveState_Implementation() const override { return bIsActive; }
	virtual void SetActiveState_Implementation(const bool bActive) override;

	/**
	 * Used for adding actors to levels or teleporting them to a new location.
	 * The result of this function is independent of the actor's current location and rotation.
	 * If the actor doesn't fit exactly at the location specified, tries to slightly move it out of walls and such if bNoCheck is false.
	 *
	 * @param DestLocation The target destination point
	 * @param DestRotation The target rotation at the destination
	 * @param bIsATest is true if this is a test movement, which shouldn't cause any notifications (used by AI pathfinding, for example)
	 * @param bNoCheck is true if we should skip checking for encroachment in the world or other actors
	 * @return true if the actor has been successfully moved, or false if it couldn't fit.
	 */
	virtual bool TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest = false, bool bNoCheck = false) override;

protected:

	/**
	 * Whether the actor is currently active.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bIsActive;

	/**
	 * Persistent identifier used to distinguish this actor.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -10))
		FGameObjectId Identity;

#if WITH_EDITORONLY_DATA
	/**
	 * Whether the actor may tick in editor viewports outside play.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	/**
	 * Called in Blueprint/C++ after the active state changes.
	 * Also called on begin play to initialize the state.
	 * @param bActive New active state.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Active State Changed")
		void ApplyActiveState(const bool bActive);
	virtual void ApplyActiveState_Implementation(const bool bActive);

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
#endif
};
