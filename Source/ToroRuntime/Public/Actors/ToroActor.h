// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/ToroActorInterface.h"
#include "ToroActor.generated.h"

/**
 * Base actor with a persistent identity and overridable activation behavior.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TORORUNTIME_API AToroActor : public AActor, public IToroActorInterface
{
	GENERATED_BODY()

public:

	AToroActor();

	virtual FGameObjectId GetIdentity_Implementation() const override { return Identity; }
	virtual bool GetActiveState_Implementation() const override { return bIsActive; }
	virtual void SetActiveState_Implementation(const bool bActive) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

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
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
#endif
};
