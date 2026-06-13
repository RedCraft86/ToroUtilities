// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Framework/ToroWorldSettings.h"
#include "Components/PostProcessComponent.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "GlobalPostProcess.generated.h"

/**
 * Manager component responsible for global post-processing effects and 
 * User-Settings feature such as Lumen GI and Reflections usage.
 * 
 * This is only supposed to be attached to the UToroWorldSettings.
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UGlobalPostProcess final : public UPostProcessComponent
{
	GENERATED_BODY()

public:

	UGlobalPostProcess();

	TORORUNTIME_API [[nodiscard]] static UGlobalPostProcess* Get(const UObject* ContextObject)
	{
		const AToroWorldSettings* WS = AToroWorldSettings::Get(ContextObject);
		return IsValid(WS) ? WS->GetGlobalPostProcess() : nullptr;
	}

	/** 
	 * Returns if Lumen Global Illumination is currently active.
	 * See bAdvancedLumenCheck for details on how this check is performed.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		TORORUNTIME_API bool IsUsingLumenGI() const { return bUsesLumenGI; }

	/** 
	 * Searches the current active blendable stack for a specific material.
	 * @param InMaterial The material interface to search for.
	 * @return The material if found in the stack (static or dynamic), otherwise nullptr.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		TORORUNTIME_API UMaterialInterface* FindBlendable(const UMaterialInterface* InMaterial) const;

	/** 
	 * Ensures a material is present in the global post-process stack.
	 * @param InMaterial The base material to add.
	 * @param bDynamic If true, creates a Material Instance Dynamic (MID) for runtime parameter modification.
	 * @return The resulting material interface (Asset or MID).
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		TORORUNTIME_API UMaterialInterface* FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic);

	/** 
	 * Removes a material from the post-process stack and tracking map.
	 * @param InMaterial The material to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		TORORUNTIME_API void RemoveBlendable(UMaterialInterface* InMaterial);

private:

	/** 
	 * If false, it will check compatibility, scalability settings, and global state to determine Lumen-GI usage.
	 * If true, it will additionally go through every post-processing volume in the world to find the exact state. 
	 */
	UPROPERTY(EditAnywhere, Category = PostProcessVolume)
		bool bAdvancedLumenCheck;

	bool bUsesLumenGI;

	void SyncUserSettings(const UToroGameUserSettings* UserSettings, const EUserSettingApplyType ApplyType);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	virtual FPostProcessVolumeProperties GetProperties() const override
	{
		FPostProcessVolumeProperties Ret;
		Ret.BlendRadius = 100.0f;
		Ret.BlendWeight = 1.0f;
		Ret.Priority = -1.0f;
		Ret.bIsUnbound = true;
		Ret.bIsEnabled = true;
		Ret.Size = DBL_MAX;
		Ret.Settings = &Settings;
		Ret.VolumeGuid = VolumeGuid;
		return Ret;
	}

	// Replica of UPostProcessComponent as they were not exported there
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostInitProperties() override;
	// ~Replica of UPostProcessComponent

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		const bool bSuper = Super::CanEditChange(InProperty);
		if (bSuper && InProperty)
		{
			return InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, Priority)
				&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, BlendRadius)
				&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, BlendWeight)
				&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, bUnbound)
				&& InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(UGlobalPostProcess, bEnabled);
		}
		return bSuper;
	}
#endif
};
