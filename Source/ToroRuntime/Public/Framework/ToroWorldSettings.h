// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Sound/SoundClass.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/WorldSettings.h"
#include "ToroWorldSettings.generated.h"

class UWorldMusicManager;
class ULightProbeManager;
class UGlobalPostProcess;
class UFlowComponent;

/**
 * Custom World Settings actor for the ToroUtilities framework.
 * Holds global Music, Post-Process, Light Probe, and Game Flow managers.
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
		static_assert(TIsDerivedFrom<T, AToroWorldSettings>::Value, 
			"T must derive from AToroWorldSettings");

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
	UFlowComponent* GetGameFlowManager() const { return GameFlow; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldMusicManager> WorldMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGlobalPostProcess> PostProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<ULightProbeManager> LightProbes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UFlowComponent> GameFlow;

	UPROPERTY(Transient, BlueprintReadOnly, Category = WorldSettings)
		TWeakObjectPtr<class UToroGameInstance> GameInstance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
