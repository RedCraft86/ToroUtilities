// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Sound/SoundClass.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/WorldSettings.h"
#include "ToroWorldSettings.generated.h"

class UWorldMusicManager;
class ULightProbeManager;
class UGlobalPostProcess;

/**
 * Custom World Settings actor for the ToroUtilities framework.
 * Automatically spawns and manages the lifecycles of the Music, Post-Process, and Light Probe managers.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (RenameCategories = "PostProcessVolume=Post Processing"))
class TORORUNTIME_API AToroWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FToroWorldSettingsDetails;
#endif
public:

	AToroWorldSettings();

	template<typename T = AToroWorldSettings>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? Cast<T>(World->GetWorldSettings()) : nullptr;
	}

	/** 
	 * Overrides the volume of a specific Sound Class globally using the default Sound Mix.
	 * @param InSoundClass Sound class to override the volume of.
	 * @param InVolume The volume multiplier to set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Audio)
		void SetSoundVolume(USoundClass* InSoundClass, float InVolume) const;

	UWorldMusicManager* GetWorldMusicManager() const { return WorldMusic; }
	UGlobalPostProcess* GetGlobalPostProcess() const { return PostProcess; }
	ULightProbeManager* GetLightProbeManager() const { return LightProbes; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldMusicManager> WorldMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGlobalPostProcess> PostProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<ULightProbeManager> LightProbes;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
