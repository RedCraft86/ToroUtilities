// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "Framework/ToroPlayerState.h"
#include "Components/AudioComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "WorldMusicManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (World), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UWorldMusicManager final : public UAudioComponent
{
	GENERATED_BODY()

public:

	UWorldMusicManager();
	
	PLAYER_COMPONENT_GETTER(UWorldMusicManager, AToroPlayerState, MusicManager)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UWorldMusicManager* GetMusicManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void ChangeMainTheme(UMetaSoundSource* InTheme);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeState(const uint8 State);

	UFUNCTION(BlueprintPure, Category = MusicManager)
		uint8 GetThemeState() const { return StateIdx; }

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeIntensity(const uint8 Intensity);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void AddDipRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void RemoveDipRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void ClearDipRequests();

	DECLARE_MULTICAST_DELEGATE(FOnMusicUpdate);
	FOnMusicUpdate OnThemeChanged;
	FOnMusicUpdate OnAudioDipped;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const uint8);
	FOnStateChanged OnStateChanged;

private:

	uint8 StateIdx;
	bool bGamePaused;
	FTimerHandle ChangeTimer;
	TSet<TWeakObjectPtr<const UObject>> DipRequests;

	void UpdateDipState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	IAudioParameterControllerInterface* GetParamInterface();
};
