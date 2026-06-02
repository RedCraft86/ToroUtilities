// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroWorldSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "LightProbeManager.generated.h"

UCLASS(NotBlueprintable, BlueprintType, PrioritizeCategories = (LightProbes), 
	HideCategories = (Tags, AssetUserData, Replication, ComponentReplication, Activation, Variable))
class TORORUNTIME_API ULightProbeManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	ULightProbeManager();

	[[nodiscard]] static ULightProbeManager* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const AToroWorldSettings* WS = IsValid(World) ? Cast<AToroWorldSettings>(World->GetWorldSettings()) : nullptr;
		return IsValid(WS) ? WS->GetLightProbeManager() : nullptr;
	}

	/** Forces light probe collection to take place NOW instead of the next interval. */
	UFUNCTION(BlueprintCallable, Category = LightProbes)
		void ForceRecollection() { UpdateInterval.ForceReady(); }

private:

	UPROPERTY(EditAnywhere, Category = LightProbes)
		TObjectPtr<UMaterialInterface> PostProcessMaterial;

	UPROPERTY(EditAnywhere, Category = LightProbes, meta = (ClampMin = 16, UIMin = 16, ClampMax = 32, UIMax = 32))
		uint8 MaxProbeCount;

	UPROPERTY(EditAnywhere, Category = LightProbes, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FSimpleCooldown UpdateInterval;

	TArray<TWeakObjectPtr<class ALightProbeActor>> LightProbes;
	TCachedGetter<UMaterialInstanceDynamic> LightProbeMID {[this] -> UMaterialInstanceDynamic* {
		if (AToroWorldSettings* WS = GetOwner<AToroWorldSettings>(); WS && PostProcessMaterial)
		{
			return Cast<UMaterialInstanceDynamic>(WS->FindOrAddBlendable(PostProcessMaterial, true));
		}
		return nullptr;
	}};

	void UpdateProbes();
	void CollectProbes();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
