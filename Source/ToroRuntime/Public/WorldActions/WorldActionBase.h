// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Tickable.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "WorldActionBase.generated.h"

/** Defines how a world state change should be applied. */
UENUM(BlueprintType)
enum class EWorldActionApplyType : uint8
{
	/** Set the value to a specific state. */
	Set,

	/** Flip the current state to its opposite. */
	Toggle
};

/**
 * Abstract base class for modular world logic. 
 * Actions can be delayed, set to run once, and are automatically assigned a World Context.
 * 
 * DESIGNER NOTE: To create a new action, inherit from this class in C++ or Blueprints.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class TORORUNTIME_API UWorldActionBase : public UObject
{
	GENERATED_BODY()

public:

	UWorldActionBase()
		: bRunOnce(false), Delay(0.0f), bRetriggerable(false), bHasRan(false)
	{}

	/** Manually sets the object used for world context (e.g., to access GetWorld()). */
	UFUNCTION(BlueprintCallable, Category = WorldAction, meta = (DefaultToSelf = "InContext"))
		void SetWorldContext(UObject* InContext);

	/** 
	 * Entry point for the action. Handles 'RunOnce' and 'Delay' logic automatically.
	 * @return True if the action successfully started (or finished if no delay).
	 */
	UFUNCTION(BlueprintCallable, Category = WorldAction)
		virtual bool RunAction();

	/** Called once when the system is initialized. Use for caching component or actor references. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Initialize")
		void OnInitialize();
	virtual void OnInitialize_Implementation() {}

	/** 
	 * Runs when the object is created or modified in the Editor. 
	 * Unlike Actors, this does NOT run automatically at BeginPlay; use OnInitialize for runtime setup.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Construction Script", meta = (ForceAsFunction = true))
		void OnConstruct();
	virtual void OnConstruct_Implementation() {}

protected:

	/** If true, subsequent calls to RunAction() will be ignored after the first successful execution. */
	UPROPERTY(EditAnywhere, Category = "Advanced")
		bool bRunOnce;

	/** Optional delay before 'OnExecute' is triggered. Values below 0.1s are treated as instant. */
	UPROPERTY(EditAnywhere, Category = "Advanced|Delay", meta = (ClampMin = 0.0f, UIMin = 0.0f, Units = "s"))
		float Delay;

	/** If a delay is active, should a new call to RunAction reset the timer (true) or be ignored (false)? */
	UPROPERTY(EditAnywhere, Category = "Advanced|Delay", meta = (EditCondition = "!bRunOnce && Delay >= 0.1"))
		bool bRetriggerable;

	/** Internal logic implementation. This is where the actual "work" of the action should be performed. */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Execute")
		void OnExecute();
	virtual void OnExecute_Implementation() {}

	bool bHasRan;
	FTimerHandle DelayTimer;
	TWeakObjectPtr<UObject> WorldContext;

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
 * An extension of WorldActionBase that receives a 'Tick' event every frame.
 * Useful for actions that require interpolation, movement, or constant polling.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class TORORUNTIME_API UTickableWorldActionBase : public UWorldActionBase, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UTickableWorldActionBase()
		: bCanTick(true), bTickWhenPaused(false)
	{}

	/** Enables or disables the ticking behavior for this specific action instance. */
	UFUNCTION(BlueprintCallable, Category = WorldAction)
		void SetTickEnabled(const bool bEnabled) { bCanTick = bEnabled; }

	/** Returns whether the action is currently allowed to tick. */
	UFUNCTION(BlueprintPure, Category = WorldAction)
		bool IsTickEnabled() const { return bCanTick; }

	/** Executed every frame while bCanTick is true. */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Tick")
		void OnTick(const float DeltaTime);
	virtual void OnTick_Implementation(const float DeltaTime) {}

protected:

	/** Master toggle for ticking. If false, OnTick will never be called. */
	UPROPERTY(EditDefaultsOnly, Category = Ticking)
		bool bCanTick;

	/** If true, the action will continue to receive OnTick events even while the Game World is paused. */
	UPROPERTY(EditDefaultsOnly, Category = Ticking, meta = (EditCondition = "bCanTick"))
		bool bTickWhenPaused;

#if WITH_EDITORONLY_DATA
	/** If true, this action will tick while the Editor is open, even if the game is not running. */
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