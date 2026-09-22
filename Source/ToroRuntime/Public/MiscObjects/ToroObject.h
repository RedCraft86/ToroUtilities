// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Tickable.h"
#include "UObject/Object.h"
#include "ToroObject.generated.h"

/**
 * Base object that supports optional world-aware ticking and Blueprint lifecycle events.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class TORORUNTIME_API UToroObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UToroObject();

	/**
	 * Enables or disables ticking for this object.
	 * @param bEnabled Whether the object should tick.
	 */
	UFUNCTION(BlueprintCallable, Category = WorldAction)
		void SetTickEnabled(const bool bEnabled);

	/**
	 * Gets whether ticking is enabled for this object.
	 */
	UFUNCTION(BlueprintPure, Category = WorldAction)
		bool IsTickEnabled() const;

	/**
	 * Initializes a newly created non-template instance.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize")
		void ReceiveInitialize();

	/**
	 * Updates the object while ticking is enabled.
	 * @param DeltaTime Time elapsed since the previous tick, in seconds.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Tick")
		void ReceiveTick(const float DeltaTime);

	virtual UWorld* GetWorld() const override;

protected:

	/**
	 * Whether the object is allowed to tick.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Ticking)
		bool bCanTick;

	/**
	 * Whether the object may tick while its world is paused.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Ticking, meta = (EditCondition = bCanTick))
		bool bTickWhenPaused;

#if WITH_EDITORONLY_DATA
	/**
	 * Whether the object may tick in editor viewports outside play.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Ticking, meta = (EditCondition = "bCanTick"))
		bool bTickInEditor = false;
#endif

	virtual void BeginDestroy() override;
	virtual void PostInitProperties() override;
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;
#if WITH_EDITOR
	virtual bool IsTickableInEditor() const override;
#endif
};
