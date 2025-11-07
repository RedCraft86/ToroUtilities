// Copyright (C) RedCraft86. All Rights Reserved.

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

	/* Whether the owning actor's ticking should also be disabled when culled */
	UPROPERTY(EditAnywhere, Category = Culling)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = Culling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = Culling)
		void RemoveRenderRequest(const UObject* Object);

private:

	TOptional<bool> bCachedTick;
	TSet<TWeakObjectPtr<const UObject>> Requests;

	void ProcessRequests();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
