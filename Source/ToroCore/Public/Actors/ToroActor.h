// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/IEnabledState.h"
#include "Interfaces/IIdentifiable.h"
#include "ToroActor.generated.h"

/**
 * A base Actor class with identity and enabled state.
 * TODO: Icon
 */
UCLASS(Abstract, Blueprintable, BlueprintType, PrioritizeCategories = (Settings, Tools), meta = (ChildCanTick = true))
class TOROCORE_API AToroActor : public AActor, public IIdentifiable, public IEnabledState
{
	GENERATED_BODY()

public:

	AToroActor();

	/** 
	 * Event broadcasted to Blueprints when the Enabled state is toggled. 
	 */
	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FEnableStateDelegateBP OnEnableStateChangedBP;

	/** 
	 * Overridable Blueprint event for handling custom logic when enabled/disabled. 
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FEnableStateDelegate OnEnableStateChanged;

	virtual void SetEnabledState_Implementation(const bool bInEnabled) override;
	virtual bool GetEnabledState_Implementation() const override { return bEnabled; }
	virtual FGameObjectId GetIdentity_Implementation() const override { return UniqueId; }

protected:

	/** 
	 * Default Scene Root 
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** 
	 * Whether this actor is currently "Enabled" in the world.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -100));
		bool bEnabled;

	/** 
	 * Identifier for saves system and other systems that involves identification.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -100))
		FGameObjectId UniqueId;

	/** 
	 * If false, collision is disabled on BeginPlay and won't be enabled by the default enabled state implementation.
	 */
	UPROPERTY(EditAnywhere, Category = Actor)
		bool bStartWithCollision;

#if WITH_EDITORONLY_DATA
	/** 
	 * If true, the Tick function will run inside the Level Editor viewport. 
	 */
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
