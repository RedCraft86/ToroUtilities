// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#if WITH_EDITOR
#include "Components/Editor/EdIconComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "Interfaces/IEnabledState.h"
#include "Interfaces/IObjectIdentity.h"
#include "ToroActor.generated.h"

/**
 * The standard base Actor for all ToroCore gameplay objects.
 * Features:
 * - Persistent Unique Identity (GUID).
 * - Unified Toggle logic (Enable/Disable).
 * - Editor-only ticking capabilities for procedural setup.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroActor : public AActor, public IObjectIdentity, public IEnabledState
{
	GENERATED_BODY()

public:

	AToroActor();

	/** Event broadcasted to Blueprints when the Enabled state is toggled. */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FEnableStateDelegateBP OnEnableStateChangedBP;

	/** Overridable Blueprint event for handling custom logic when enabled/disabled. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FEnableStateDelegate OnEnableStateChanged;

	virtual FGameObjectId GetGameObjectId_Implementation() const override { return UniqueId; }
	virtual bool GetEnabledState_Implementation() const override { return bEnabled; }
	virtual void SetEnabledState_Implementation(const bool bInEnabled) override;

protected:

	/** Default Scene Root */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** Whether this actor is currently "Active" in the world. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -100));
		bool bEnabled;

	/** 
	 * Unique Identifier for save systems and cross-actor referencing.
	 * Marked as Transient for duplication/export to ensure IDs remain unique.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

	/** If false, collision is automatically disabled on BeginPlay. */
	UPROPERTY(EditAnywhere, Category = Actor)
		bool bStartWithCollision;

#if WITH_EDITORONLY_DATA
	/** If true, the Tick function will run inside the Level Editor viewport. */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

#if WITH_EDITOR
	DECLARE_ICON_COMPONENT
#endif

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
