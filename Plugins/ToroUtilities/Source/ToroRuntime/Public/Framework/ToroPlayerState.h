// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroPlayerState.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AToroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AToroPlayerState();
	
	PLAYER_CLASS_GETTER(AToroPlayerState, GetPlayerState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UWorldMusicManager> MusicManager;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UTutorialManager> Tutorials;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInventoryManager> Inventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInspectionManager> Inspection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UNarrativeManager> Narrative;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAchievementManager> Achievements;
};
