// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DataTypes/RequesterSet.h"
#include "Components/ActorComponent.h"
#include "LazyRenderComponent.generated.h"

/**
 * A component focused on optimization where the owning actor will only be visible if there are requests.
 * Ideally made for lights where you can stop it from rendering once you leave the room it's in for example.
 */
UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class ULazyRenderComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	ULazyRenderComponent();

	/**
	 * Looks for the lazy render component and adds a requester to keep this Actor rendered.
	 * Actor remains visible as long as at least one valid requester exists in the set.
	 * @param Target The actor to look for the lazy render component.
	 * @param InRequester The object requesting the render state.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering, meta = (DefaultToSelf = Target))
		static TOROCORE_API void AddRenderRequest(const AActor* Target, const UObject* InRequester);

	/**
	 * Looks for the lazy render component and removes a requester.
	 * If no requesters remain in the list, the actor will be hidden.
	 * @param Target The actor to look for the lazy render component.
	 * @param InRequester The object that originally made the request.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering, meta = (DefaultToSelf = Target))
		static TOROCORE_API void RemoveRenderRequest(const AActor* Target, const UObject* InRequester);

	/**
	 * Adds a requester to keep this Actor rendered.
	 * Actor remains visible as long as at least one valid requester exists in the set.
	 * @param InRequester The object requesting the render state.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering)
		TOROCORE_API void AddRequest(const UObject* InRequester);

	/**
	 * Removes a requester to potentially stop rendering this actor.
	 * If no requesters remain in the list, the actor will be hidden.
	 * @param InRequester The object that originally made the request.
	 */
	UFUNCTION(BlueprintCallable, Category = LazyRendering)
		TOROCORE_API void RemoveRequest(const UObject* InRequester);

protected:

	FRequesterSet Requests;

	void OnRequestChanged(const bool bState) const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
