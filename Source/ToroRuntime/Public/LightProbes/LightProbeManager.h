// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "DataTypes/CachedGetter.h"
#include "DataTypes/SimpleCooldown.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroWorldSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "LightProbeManager.generated.h"

/**
 * Manages the collection and buffering of Post-Process based light probe actors in the scene.
 * Syncs probe data to a Material Instance Dynamic for use in global lighting materials.
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class ULightProbeManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	ULightProbeManager();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Light Probe Manager"))
	static TORORUNTIME_API ULightProbeManager* Get(const UObject* ContextObject)
	{
		const AToroWorldSettings* WS = AToroWorldSettings::Get(ContextObject);
		return IsValid(WS) ? WS->GetLightProbeManager() : nullptr;
	}

	/** 
	 * Forces an immediate scan and update of light probe data. 
	 * Useful after level streaming or dynamic actor spawning.
	 */
	UFUNCTION(BlueprintCallable, Category = LightProbes)
		TORORUNTIME_API void ForceRecollection() { UpdateInterval.ForceReady(); }

private:

	/** The master material used for the lighting pass that receives probe data. */
	UPROPERTY(EditAnywhere, Category = LightProbes)
		TObjectPtr<UMaterialInterface> PostProcessMaterial;

	/** Maximum allowed light probes. Clamped to 16-32 to match shader buffer limits. */
	UPROPERTY(EditAnywhere, Category = LightProbes, meta = (ClampMin = 16, UIMin = 16, ClampMax = 32, UIMax = 32))
		uint8 MaxProbeCount;

	/** Refresh interval to avoid costly collection logic every frame. */
	UPROPERTY(EditAnywhere, Category = LightProbes, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FSimpleCooldown UpdateInterval;

	TArray<TWeakObjectPtr<class ALightProbeActor>> LightProbes;
	TCachedGetter<UMaterialInstanceDynamic> LightProbeMID {[this]() {
		return CreateLightProbeMID();
	}};

	void UpdateProbes();
	void CollectProbes();
	UMaterialInstanceDynamic* CreateLightProbeMID() const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
