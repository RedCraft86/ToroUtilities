// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "ToroDataNode.h"
#include "ToroDataGraph.h"
#include "MetasoundSource.h"
#include "Actors/TeleportPoint.h"
#include "DataTypes/LightingData.h"
#include "Helpers/GameplayTagMacros.h"
#include "Inventory/InventoryAsset.h"
#include "MiscObjects/UDSSetterObject.h"
#include "GamePhaseData.generated.h"

namespace ContentTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Content)
	TORORUNTIME_API CREATE_TAG_VERIFIER(Content)
}

UCLASS(Abstract)
class TORORUNTIME_API UToroGamePhaseGraph : public UToroDataGraph
{
	GENERATED_BODY()

public:

	UToroGamePhaseGraph();
};

UCLASS(Abstract, PrioritizeCategories = (Node, Loading, Level, Game, Player, Inventory))
class TORORUNTIME_API UToroGamePhaseNode : public UToroDataNode
{
	GENERATED_BODY()

public:

	UToroGamePhaseNode();

	UPROPERTY(EditAnywhere, Category = Loading)
		bool bSimpleLoading;

	UPROPERTY(EditAnywhere, Category = Loading, meta = (EditCondition = "!bSimpleLoading"))
		TArray<TSoftObjectPtr<UTexture2D>> Backgrounds;

	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, Category = Level)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = Game)
		FName PostLoadEvent;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftClassPtr<UQuest> Quest;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftObjectPtr<UMetaSoundSource> Theme;

	UPROPERTY(EditAnywhere, Category = Game, meta = (Categories = "Content"))
		FGameplayTagContainer UnlockContent;

	UPROPERTY(EditAnywhere, Category = Game, DisplayName = "Sky & Weather")
		FUDSSettings SkyWeather;

	UPROPERTY(EditAnywhere, Category = Player, meta = (DisplayPriority = -1))
		TSoftObjectPtr<ATeleportPoint> TeleportPoint;

	UPROPERTY(EditAnywhere, Category = Player, meta = (DisplayPriority = 1))
		FPointLightProperties PlayerLight;

	UPROPERTY(EditAnywhere, Category = Inventory, meta = (Categories = "Inventory"))
		FGameplayTag InventoryProfile;

	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<TSoftObjectPtr<UInventoryAsset>> Archives;

	UPROPERTY(EditAnywhere, Category = Inventory, meta = (ClampMin = 1, UIMin = 1))
		TMap<TSoftObjectPtr<UInventoryAsset>, uint8> Items;

	TSet<FGameplayTag> GetContentTags() const;
	TSet<TSoftObjectPtr<UWorld>> GetLevels() const;
	TSoftObjectPtr<UTexture2D> GetBackground() const;
	
	virtual void TeleportPlayer() const;
	virtual void ApplyPlayerSettings(class AToroPlayerCharacter* PlayerChar) const;

#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
