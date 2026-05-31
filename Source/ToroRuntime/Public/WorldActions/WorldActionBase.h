// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Tickable.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "WorldActionBase.generated.h"

UENUM(BlueprintType)
enum class EWorldActionApplyType : uint8
{
	Set,
	Toggle
};

/**
 * Abstract base class for modular and polymorphic actions to apply simple changes to the game world.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class TORORUNTIME_API UWorldActionBase : public UObject
{
	GENERATED_BODY()

public:

	UWorldActionBase()
		: bRunOnce(false), Delay(0.0f), bRetriggerable(false), bHasRan(false)
	{}

	UFUNCTION(BlueprintCallable, Category = WorldAction, meta = (DefaultToSelf = "InContext"))
		void SetWorldContext(const UObject* InContext);

	UFUNCTION(BlueprintCallable, Category = WorldAction)
		virtual bool RunAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Initialize")
		void OnInitialize();
	virtual void OnInitialize_Implementation() {}

	/* NOTE: Unlike Actors, the Construction Script is not re-ran on begin play. */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Construction Script", meta = (ForceAsFunction = true))
		void OnConstruct();
	virtual void OnConstruct_Implementation() {}

protected:

	/* Run this action only once. */
	UPROPERTY(EditAnywhere, Category = "Advanced")
		bool bRunOnce;

	/* Time to wait before executing the action. Only applies if value is 0.1 or greater. */
	UPROPERTY(EditAnywhere, Category = "Advanced|Delay", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float Delay;

	/* If during a delay, whether subsequent run calls should be ignored or retrigger (reset) the delay. */
	UPROPERTY(EditAnywhere, Category = "Advanced|Delay", meta = (EditCondition = "!bRunOnce && Delay >= 0.1"))
		bool bRetriggerable;

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Execute")
		void OnExecute();
	virtual void OnExecute_Implementation() {}

	bool bHasRan;
	FTimerHandle DelayTimer;
	TWeakObjectPtr<const UObject> WorldContext;

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		OnConstruct();
	}
#endif
};

/**
 * UWorldActionBase with ticking capabilities.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class TORORUNTIME_API UTickableWorldActionBase : public UWorldActionBase, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UTickableWorldActionBase()
		: bCanTick(true), bTickWhenPaused(false)
	{}

	UFUNCTION(BlueprintCallable, Category = WorldAction)
		void SetTickEnabled(const bool bEnabled) { bCanTick = bEnabled; }

	UFUNCTION(BlueprintPure, Category = WorldAction)
		bool IsTickEnabled() const { return bCanTick; }

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Tick")
		void OnTick(const float DeltaTime);
	virtual void OnTick_Implementation(const float DeltaTime) {}

protected:

	/* Whether this object can currently tick. */
	UPROPERTY(EditDefaultsOnly, Category = Ticking)
		bool bCanTick;

	/* Whether this object can tick even if the game is paused (if also bCanTick = true). */
	UPROPERTY(EditDefaultsOnly, Category = Ticking, meta = (EditCondition = "bCanTick"))
		bool bTickWhenPaused;

#if WITH_EDITORONLY_DATA
	/* Whether this object can tick in the editor (if also bCanTick = true). */
	UPROPERTY(EditDefaultsOnly, Category = Ticking, meta = (EditCondition = "bCanTick"))
		bool bTickInEditor = false;
#endif

	virtual void Tick(float DeltaTime) override { OnTick(DeltaTime); }
	virtual bool IsTickable() const override { return bCanTick && !IsTemplate(); }
	virtual bool IsTickableWhenPaused() const override { return bTickWhenPaused; }
	virtual TStatId GetStatId() const override { return GetStatID(); }
#if WITH_EDITOR
	virtual bool IsTickableInEditor() const override { return bTickInEditor; }
#endif
};