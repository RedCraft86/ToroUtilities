// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "MetasoundSource.h"
#include "DataTypes/RequesterSet.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroWorldSettings.h"
#include "WorldMusicManager.generated.h"

/**
 * A specialized Audio Component that manages the global background music state.
 * Supports MetaSound parameter injection, fading via transitions, and 
 * volume "dipping" triggered by gameplay events.
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

	/** 
	 * Transitions the current music to a new MetaSound source.
	 * Fades out current track in half transition time and fades in new track in half transition time.
	 * If this is the first track, fades in with full transition time.
	 * @param NewSource The MetaSound asset to play.
	 */
	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void ChangeMusic(UMetaSoundSource* NewSource);

	/** 
	 * Adds a request to "dip" (lower) the music volume and apply low-pass filters.
	 * Music remains dipped as long as at least one valid requester exists in the set.
	 * @param InRequester The object requesting the dip (e.g., a Dialogue System or UI).
	 */
	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void AddDipRequest(const UObject* InRequester);

	/** 
	 * Removes a dip request. If no requesters remain, music returns to full volume.
	 * @param InRequester The object that originally made the request.
	 */
	UFUNCTION(BlueprintCallable, Category = WorldMusic)
		TORORUNTIME_API void RemoveDipRequest(const UObject* InRequester);

private:

	/** The default theme to play initially. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic")
		TObjectPtr<UMetaSoundSource> DefaultTheme;

	/** Duration in seconds for fading between music tracks. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic")
		float TransitionTime;

	/** 
	 * If true, periodically scans the request list for stale/null pointers.
	 * Prevents music from being stuck in a dipped state if a requester is destroyed.
	 */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping")
		bool bDipNullChecks;

	/** The volume multiplier to apply when music is in a 'Dipped' state. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping", meta = (ClampMin = 0.1f))
		float DippedVolume;

	/** The low-pass filter frequency (Hz) to apply when dipped (simulates muffling). */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Dipping", meta = (ClampMin = 0.0f))
		float DippedLowPassFilter;

	/** Default boolean parameters to push to the MetaSound instance on theme change. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Booleans")
		TMap<FName, bool> DefaultBoolParams;

	/** Default integer parameters to push to the MetaSound instance on theme change. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Parameters Integers")
		TMap<FName, int32> DefaultInt32Params;

	/** Default float parameters to push to the MetaSound instance on theme change. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DisplayName = "Default Parameters Floats")
		TMap<FName, float> DefaultFloatParams;

#if WITH_EDITORONLY_DATA
	/** Pull Boolean, Integer, and Float parameters from the current Default Theme. */
	UPROPERTY(EditAnywhere, Category = "WorldMusic|Parameters", DuplicateTransient, TextExportTransient, meta = (DisplayPriority = -1))
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
