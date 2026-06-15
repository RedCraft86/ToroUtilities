// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "DataTypes/RequesterSet.h"
#include "Components/ToroComponents.h"
#include "LazyRenderComponent.generated.h"

/**
 * A performance-focused component that toggles the Actor's rendering state (HiddenInGame) 
 * based on a set of active requests.
 * 
 * Concept: 
 * The Actor remains invisible/unrendered by default. It only becomes visible when one or 
 * more systems (Requesters) explicitly register a request. This pattern is highly effective 
 * for localized rendering systems, proximity-based activations, or cinematic-specific actors.
 */
UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class ULazyRenderComponent final : public UToroActorComponent
{
	GENERATED_BODY()

public:

	ULazyRenderComponent();

	/** 
	 * Adds a requester to keep this Actor rendered. 
	 * Actor remains visible as long as at least one valid requester exists in the set.
	 * @param InRequester The object (e.g., a Trigger Volume or Camera) requesting the render state.
	 */
	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		TORORUNTIME_API void AddRequest(const UObject* InRequester);

	/** 
	 * Removes a requester. If no requesters remain, the actor will be hidden.
	 * @param InRequester The object that originally made the request.
	 */
	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		TORORUNTIME_API void RemoveRequest(const UObject* InRequester);

protected:

	/** 
	 * If true, periodically scans the request list for stale/null pointers.
	 * Prevents actor from being stuck in a rendered state if a requester is destroyed.
	 */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bNullChecks;

	FRequesterSet Requests;

	void OnRequestChanged(const bool bState) const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
