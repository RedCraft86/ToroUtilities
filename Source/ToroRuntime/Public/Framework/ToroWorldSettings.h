// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Sound/SoundClass.h"
#include "Helpers/WorldGetter.h"
#include "DataTypes/CachedGetter.h"
#include "GameFramework/WorldSettings.h"
#include "Components/PostProcessComponent.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "ToroWorldSettings.generated.h"

/**
 * Custom implementation of AWorldSettings that serves as the centralized visual 
 * and configuration baseline for the level. Also hosts a global UPostProcessComponent 
 * with a dynamic Post-Process Material (Blendable) management.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:

	AToroWorldSettings();

	template<typename T = AToroWorldSettings>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? Cast<T>(World->GetWorldSettings()) : nullptr;
	}

	/** 
	 * Checks if Lumen GI is currently enabled. Returns false if GI Quality is Low. 
	 * @note Assumes it is on by default in the project and does NOT account for local post process volumes.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		bool IsUsingLumenGI();

	/** 
	 * Accesses the underlying PostProcessSettings structure.
	 * @return Reference to the final global post-processing configuration.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		const FPostProcessSettings& GetPostProcessSettings() const;

	/** 
	 * Searches the current active blendable stack for a specific material.
	 * @param InMaterial The material interface to search for.
	 * @return The material if found in the stack (static or dynamic), otherwise nullptr.
	 */
	UFUNCTION(BlueprintPure, Category = PostProcess)
		UMaterialInterface* FindBlendable(const UMaterialInterface* InMaterial) const;

	/** 
	 * Ensures a material is present in the global post-process stack.
	 * @param InMaterial The base material to add.
	 * @param bDynamic If true, creates a Material Instance Dynamic (MID) for runtime parameter modification.
	 * @return The resulting material interface (Asset or MID).
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		UMaterialInterface* FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic);

	/** 
	 * Removes a material from the post-process stack and tracking map.
	 * @param InMaterial The material to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = PostProcess)
		void RemoveBlendable(UMaterialInterface* InMaterial);

	/** 
	 * Overrides the volume of a specific Sound Class globally using the default Sound Mix.
	 * @param InSoundClass Sound class to override the volume of.
	 * @param InVolume The volume multiplier to set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Audio)
		void SetSoundVolume(USoundClass* InSoundClass, float InVolume) const;

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPostProcessComponent> PostProcess;

	/** Time (in seconds) between user-setting synchronization. 0.0 for every frame (Runtime Only). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PostProcessing, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float PostProcessTickInterval;

	/** The baseline post-processing configuration for this specific world. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PostProcessing, meta = (ShowOnlyInnerProperties))
		FPostProcessSettings PostProcessing;

	float PostProcessTick;
	TCachedGetter<UToroGameUserSettings> UserSettings {[]
	{
		return UToroGameUserSettings::Get();
	}};

	void UpdatePostProcess();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
