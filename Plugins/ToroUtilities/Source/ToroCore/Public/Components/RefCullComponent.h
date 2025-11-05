// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/ActorComponent.h"
#include "RefCullComponent.generated.h"

/**
 * A component that handles hiding (and/or ticking) of its parent actor depending on if there are any requests for it.
 */
UCLASS(NotBlueprintable, DisplayName = "Reference Culling", ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROCORE_API URefCullComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	URefCullComponent();

	/* Do periodic checks to ensure all requests are non-null. Otherwise, only check when adding or removing one. */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bNullChecks;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void AddRequest(const UObject* InRequester);

	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void RemoveRequest(const UObject* InRequester);

protected:

	TOptional<bool> OriginalTickState;
	TSet<TWeakObjectPtr<const UObject>> Requests;

	void UpdateCullingState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
