// Copyright (C) RedCraft86. All Rights Reserved.

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

	UPROPERTY(EditAnywhere, Category = "Settings|Culling", DisplayName = "Invert")
		bool CullInvert;

	UPROPERTY(EditAnywhere, Category = "Settings|Culling", DisplayName = "Targets")
		TSet<TSoftObjectPtr<AActor>> CullTargets;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions", DisplayName = "On Enter")
		FWorldActionArray ActionsEnter;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions", DisplayName = "On Exit")
		FWorldActionArray ActionsExit;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Bounded")
		bool CullFindBounded = true;

	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Find Tag")
		FName CullFindTag = NAME_None;
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools)
		void FindCullTargets();
#endif

	FTimerHandle CullingTimer;
	TObjectPtr<UWorldMusicManager> MusicManager;
	TObjectPtr<APlayerCameraManager> CamManager;

	void UpdateRefCulling();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
