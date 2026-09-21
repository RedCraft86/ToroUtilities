// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "GameFramework/Volume.h"
#include "Interfaces/ToroActorInterface.h"
#include "ToroVolume.generated.h"

/**
 * Base volume with a persistent identity and overridable activation behavior.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TORORUNTIME_API AToroVolume : public AVolume, public IToroActorInterface
{
	GENERATED_BODY()

public:

	AToroVolume();

	virtual FGameObjectId GetIdentity_Implementation() const override { return Identity; }
	virtual bool GetActiveState_Implementation() const override { return bIsActive; }
	virtual void SetActiveState_Implementation(const bool bActive) override;

protected:

	/**
	 * Whether the volume is currently active.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bIsActive;

	/**
	 * Persistent identifier used to distinguish this volume.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -10))
		FGameObjectId Identity;

#if WITH_EDITORONLY_DATA
	/**
	 * Whether the volume may tick in editor viewports outside play.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;

	/**
	 * Enables custom color overrides for the volume wireframe.
	 */
	UPROPERTY(EditAnywhere, Category = BrushSettings, DisplayName = "Colored", meta = (InlineEditConditionToggle))
		bool bColoredVolume = false;

	/**
	 * The color used for the volume bounds if bColoredVolume is true.
	 */
	UPROPERTY(EditAnywhere, Category = BrushSettings, DisplayName = "Color", meta = (EditCondition = bColoredVolume, HideAlphaChannel = true))
		FColor VolumeColor = FColor(124, 75, 155);
#endif

	/**
	 * Called in Blueprint after the active state changes.
	 * Also called on begin play to initialize the state.
	 * @param bActive New active state.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Active State Changed")
		void ApplyActiveState(const bool bActive);
	virtual void ApplyActiveState_Implementation(const bool bActive);

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
#endif
};
