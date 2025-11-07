// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Components/EditorVisualComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "DataTypes/ToroDelegates.h"
#include "Interfaces/ActorStateInterface.h"
#include "Interfaces/GuidInterface.h"
#include "ToroActor.generated.h"

UCLASS(Abstract, HideCategories = (Replication, Networking, Mobile, Input, RayTracing, Cooking), PrioritizeCategories = (Settings, Tools))
class TOROCORE_API AToroActor : public AActor, public IActorStateInterface, public IGuidInterface
{
	GENERATED_BODY()

public:

	AToroActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FToroBoolDelegateBP OnEnableStateChangedBP;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FToroBoolDelegate OnEnableStateChanged;

	DECLARE_ENABLE_STATE_HELPERS
	virtual void SetEnabledState_Implementation(const bool bInEnabled) override;
	virtual bool GetEnabledState_Implementation() const override { return bEnabled; }
	virtual FGuid GetUniqueGUID_Implementation() override { return UniqueGuid; }

protected:
#if WITH_EDITOR
	DECLARE_VISUAL_COMPONENT
#endif
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid UniqueGuid;

	UPROPERTY(EditAnywhere, Category = Actor)
		bool bStartWithCollision;

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
