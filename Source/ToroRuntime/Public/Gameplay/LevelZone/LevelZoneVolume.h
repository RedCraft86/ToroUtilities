// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Actors/ToroVolume.h"
#include "GameplayTagContainer.h"
#include "MusicSystem/WorldMusicManager.h"
#include "WorldActions/WorldActionManager.h"
#include "LevelZoneVolume.generated.h"

UCLASS()
class TORORUNTIME_API ALevelZoneVolume final : public AToroVolume
{
	GENERATED_BODY()
#if WITH_EDITOR
	friend class FZoneVolumeDetails;
#endif
public:

	ALevelZoneVolume();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionManager> ActionManager;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (Categories = "Zone"))
		FGameplayTag ZoneTag;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float ThemeIntensity;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions", DisplayName = "On Enter")
		FWorldActionArray ActionsEnter;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions", DisplayName = "On Exit")
		FWorldActionArray ActionsExit;

	TObjectPtr<UWorldMusicManager> MusicManager;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
