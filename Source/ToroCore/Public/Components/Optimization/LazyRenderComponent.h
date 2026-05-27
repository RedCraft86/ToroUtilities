// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/ToroComponents.h"
#include "LazyRenderComponent.generated.h"

/**
 * A performance-focused component that toggles the Actor's rendering state based on active requests.
 * 
 * Concept: The Actor remains invisible/unrendered unless one or more systems (Requesters) 
 * explicitly ask for it to be rendered. This is highly effective for localized rendering systems.
 */
UCLASS(NotBlueprintable, DisplayName = "Lazy Renderer", ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROCORE_API ULazyRenderComponent final : public UToroActorComponent
{
	GENERATED_BODY()

public:

	ULazyRenderComponent();

	/** 
	 * If true, periodically scans the request list for stale/null pointers.
	 * Recommended for systems where requesters might be destroyed without calling RemoveRequest.
	 */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bNullChecks;

	/** 
	 * Registers a requester. If this is the first requester, the Actor's rendering is enabled.
	 * @param InRequester The object requesting the render state.
	 */
	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void AddRequest(const UObject* InRequester);

	/** 
	 * Unregisters a requester. If no requesters remain, the Actor's rendering is disabled.
	 * @param InRequester The object releasing its request.
	 */
	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void RemoveRequest(const UObject* InRequester);

protected:

	TSet<TWeakObjectPtr<const UObject>> Requests;

	void UpdateRenderState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
