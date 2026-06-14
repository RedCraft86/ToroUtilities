// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "MetasoundSource.h"
#include "DataTypes/RequesterSet.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroWorldSettings.h"
#include "WorldMusicManager.generated.h"

/**
 * Manager component responsible for global music playing
 * 
 * This is only supposed to be attached to the UToroWorldSettings.
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UWorldMusicManager final : public UAudioComponent
{
	GENERATED_BODY()

public:

	UWorldMusicManager();

	TORORUNTIME_API [[nodiscard]] static UWorldMusicManager* Get(const UObject* ContextObject)
	{
		const AToroWorldSettings* WS = AToroWorldSettings::Get(ContextObject);
		return IsValid(WS) ? WS->GetWorldMusicManager() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void ChangeMusic(UMetaSoundSource* NewSource);

	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void AddDipRequest(const UObject* InRequester);

	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void RemoveDipRequest(const UObject* InRequester);

private:

	UPROPERTY(EditAnywhere, Category = "WorldMusic")
		TObjectPtr<UMetaSoundSource> DefaultTheme;

	UPROPERTY(EditAnywhere, Category = "WorldMusic")
		float TransitionTime;

	/** 
	 * If true, periodically scans the request list for stale/null pointers.
	 * Recommended for systems where requesters might be destroyed without calling RemoveDipRequest.
	 */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping")
		bool bDipNullChecks;

	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping", meta = (ClampMin = 0.1f))
		float DippedVolume;

	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping", meta = (ClampMin = 0.0f))
		float DippedLowPassFilter;

	// default params to apply when changing themes
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Booleans")
		TMap<FName, bool> DefaultBoolParams;

	// default params to apply when changing themes
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Parameters Integers")
		TMap<FName, int32> DefaultInt32Params;

	// default params to apply when changing themes
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Parameters Floats")
		TMap<FName, float> DefaultFloatParams;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "WorldMusic", AdvancedDisplay, DuplicateTransient, TextExportTransient)
		bool bRefreshParameters = true;
#endif

	FTimerHandle TransitionTimer;
	FRequesterSet DipRequests;

	void ResetSoundParams();
	void OnDipRequestChanged(const bool bState);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
