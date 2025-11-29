// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/ActorComponent.h"
#include "CullingComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UCullingComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UCullingComponent();

	static UCullingComponent* Get(const AActor* Target);

	/* Whether the owning actor's Visibility should also be disabled when culled */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAffectVisibility;

	/* Whether the owning actor's ticking should also be disabled when culled */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAffectTicking;

	/* Whether the owning actor's EnabledState should also be disabled when culled */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAffectState;

	UFUNCTION(BlueprintCallable, Category = Culling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = Culling)
		void RemoveRenderRequest(const UObject* Object);
	
	UFUNCTION(BlueprintCallable, Category = Culling)
		void ProcessRequests();

	UFUNCTION(BlueprintCallable, Category = Culling)
		bool IsOwnerCulled() const { return bCulled; }

private:

	bool bCulled;
	bool bCachedTick;
	TSet<TWeakObjectPtr<const UObject>> Requests;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
