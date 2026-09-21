// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroActorComponent.h"
#include "DataTypes/AccumulatorSet.h"
#include "LazyRenderComponent.generated.h"

/**
 * Hides its owning actor until at least one valid object requests rendering.
 */
UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class ULazyRenderComponent final : public UToroActorComponent
{
	GENERATED_BODY()

public:

	ULazyRenderComponent();

	/**
	 * Adds a render request to an actor's lazy render component.
	 * @param Target Actor whose rendering is requested.
	 * @param InRequester Object that owns the request.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering, meta = (DefaultToSelf = Target))
		static TORORUNTIME_API void AddRenderRequest(const AActor* Target, const UObject* InRequester);

	/**
	 * Removes a render request from an actor's lazy render component.
	 * @param Target Actor whose request should be removed.
	 * @param InRequester Object that owns the request.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering, meta = (DefaultToSelf = Target))
		static TORORUNTIME_API void RemoveRenderRequest(const AActor* Target, const UObject* InRequester);

	/**
	 * Adds an object to this component's render request set.
	 * @param InRequester Object that owns the request.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = LazyRendering)
		TORORUNTIME_API void AddRequest(const UObject* InRequester) const;

	/**
	 * Removes an object from this component's render request set.
	 * @param InRequester Object that owns the request.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = LazyRendering)
		TORORUNTIME_API void RemoveRequest(const UObject* InRequester) const;

protected:

	TSharedRef<TAccumulatorSet<TWeakObjectPtr<const UObject>>> Requests;

	void OnRequestChanged() const;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
};
