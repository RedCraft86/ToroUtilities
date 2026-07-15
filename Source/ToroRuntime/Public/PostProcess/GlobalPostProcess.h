// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Framework/ToroWorldSettings.h"
#include "Components/PostProcessComponent.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "GlobalPostProcess.generated.h"

/**
 * A customized Post Process Component that stays active across the entire world (Unbound).
 * Used to centralize global effects and provide a unified API for blendable materials.
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UGlobalPostProcess final : public UPostProcessComponent
{
	GENERATED_BODY()

public:

	UGlobalPostProcess();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Global Post Process"))
	static TORORUNTIME_API UGlobalPostProcess* Get(const UObject* ContextObject)
	{
		const AToroWorldSettings* WS = AToroWorldSettings::Get(ContextObject);
		return IsValid(WS) ? WS->GetGlobalPostProcess() : nullptr;
	}

	/** 
	 * Returns true if Lumen Global Illumination is effectively active in the renderer.
	 * Considers scalability, project settings, and active volumes.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		TORORUNTIME_API bool IsUsingLumenGI() const { return bUsesLumenGI; }

	/** 
	 * Checks if a specific material (or an instance of it) is currently in the blendable stack.
	 * @param InMaterial The material asset to search for.
	 * @return The material found in the stack, or nullptr.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		TORORUNTIME_API UMaterialInterface* FindBlendable(const UMaterialInterface* InMaterial) const;

	/** 
	 * Ensures a material is in the post-process stack. If it doesn't exist, it is added.
	 * If it already exists, its weight is set back to 1.
	 * @param InMaterial The material asset to add.
	 * @param bDynamic If true, ensures the stack contains a Material Instance Dynamic (MID).
	 * @return The active Material Interface (Asset or MID).
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		TORORUNTIME_API UMaterialInterface* FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic);

	/** 
	 * Removes a specific material (or an instance of it) from the global blendable stack.
	 * @param InMaterial The material to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		TORORUNTIME_API void RemoveBlendable(UMaterialInterface* InMaterial);

private:

	/** 
	 * If true, the Lumen check will perform a deep scan of all volumes in the world.
	 * If false, it relies on global engine state and component-level settings.
	 */
	UPROPERTY(EditAnywhere, Category = PostProcessVolume)
		bool bAdvancedLumenCheck;

	bool bUsesLumenGI;

	void SyncUserSettings(const UToroGameUserSettings* UserSettings, const EUserSettingApplyType ApplyType);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	virtual FPostProcessVolumeProperties GetProperties() const override;

	// Overrides to maintain parity with UPostProcessComponent internal logic
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostInitProperties() override;

#if WITH_EDITOR	
	/** Prevents manual editing of standard PP properties that are managed internally. */
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};
