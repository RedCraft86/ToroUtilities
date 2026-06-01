// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/TargetPoint.h"
#include "TeleportPoint.generated.h"

/**
 * A specialized Target Point used for precise actor and player teleportation.
 * Includes editor utilities to automatically align the point with the floor, 
 * accounting for character collision half-heights or mesh offsets.
 */
UCLASS(MinimalAPI, NotBlueprintable, meta = (HideCategories = "*", ShowCategories = "Tools"))
class ATeleportPoint final : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportPoint();

	/** 
	 * Teleports the specified actor to this point's location and rotation.
	 * @param TargetActor The actor to move. If a ToroCharacter, its own Teleport function is used.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TORORUNTIME_API void TeleportActor(AActor* TargetActor) const;

	/** 
	 * Teleports a specific player (via index) to this point.
	 * @param PlayerIndex The local player index (usually 0 for the primary player).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		TORORUNTIME_API void TeleportPlayer(const int32 PlayerIndex = 0) const;

private:
#if WITH_EDITORONLY_DATA
	/** 
	* The vertical offset applied when 'Offset From Floor' is called.
	* Default of 88.0 units is standard for the AToroPlayerCharacter capsule half-height.
	*/
	UPROPERTY(EditAnywhere, Category = Tools)
		float FloorOffset = 88.0f;

	/** 
	 * DO NOT CHANGE. Internal flag to track if an automatic offset has already been applied.
	 */
	UPROPERTY(EditAnywhere, Category = Tools, AdvancedDisplay)
		bool bDidOffset = false;
#endif

#if WITH_EDITOR
	/** 
	 * Utility button to snap this actor to the floor below it and apply the FloorOffset.
	 * This prevents players from spawning inside the ground or falling from the sky.
	 */
	UFUNCTION(CallInEditor, Category = Tools)
		void OffsetFromFloor();

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
