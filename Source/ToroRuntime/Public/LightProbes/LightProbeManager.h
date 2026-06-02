// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroWorldSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "LightProbeManager.generated.h"

/**
 * Manager component responsible for tracking light probe actors and updating 
 * global post-process material parameters for custom lighting effects.
 * 
 * This is only supposed to be attached to the UToroWorldSettings.
 */
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

	/** 
	 * Forces the manager to search for LightProbeActors and update the shader buffer immediately. 
	 * Bypasses the standard UpdateInterval cooldown.
	 */
	UFUNCTION(BlueprintCallable, Category = LightProbes)
		void ForceRecollection() { UpdateInterval.ForceReady(); }

private:

	/** The material used for the lighting post-process pass. */
	UPROPERTY(EditAnywhere, Category = LightProbes)
		TObjectPtr<UMaterialInterface> PostProcessMaterial;

	/** Maximum number of light probes supported by the shader buffer (Clamped 16-32). */
	UPROPERTY(EditAnywhere, Category = LightProbes, meta = (ClampMin = 16, UIMin = 16, ClampMax = 32, UIMax = 32))
		uint8 MaxProbeCount;

	/** Cooldown interval between light probe data refreshes to optimize performance. */
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
