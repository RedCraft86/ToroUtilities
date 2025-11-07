// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "LocalMusicVolume.generated.h"

UCLASS(PrioritizeCategories = (Settings))
class TORORUNTIME_API ALocalMusicVolume final : public AToroVolume
{
	GENERATED_BODY()

public:

	ALocalMusicVolume();

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.0f, ClampMax = 0.0f))
		float Cooldown;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.1f, ClampMax = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.0f, ClampMax = 0.0f))
		FVector2D StartRange;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.1f, ClampMax = 0.1f))
		FVector2D FadeInOut;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDipMainTheme;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<uint8> AllowedStates;

private:

	UPROPERTY() TObjectPtr<UAudioComponent> SoundComponent;

	bool bInVolume;
	bool bOnCooldown;
	uint8 MusicState;
	TObjectPtr<class UWorldMusicManager> Manager;

	void CooldownFinish()
	{
		bOnCooldown = false;
		UpdateAudio();
	}
	void StopUntilNextTrigger()
	{
		bInVolume = false;
		UpdateAudio();
	}
	void OnMusicStateChanged(const uint8 State)
	{
		MusicState = State;
		UpdateAudio();
	}

	void UpdateAudio();
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
