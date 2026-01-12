// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "GamePhase/GamePhaseData.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Inventory/InventoryManager.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

namespace ContentTags
{
	DEFINE_GAMEPLAY_TAG(Content)
}

UToroGamePhaseGraph::UToroGamePhaseGraph()
{
#if WITH_EDITOR
	CompatibleNode = UToroGamePhaseNode::StaticClass();
	CompatibleEdge = nullptr;
#endif
}

UToroGamePhaseNode::UToroGamePhaseNode(): bSimpleLoading(false)
{
	Name = NSLOCTEXT("Game", "NewPhaseName", "New Phase");
	Description = NSLOCTEXT("Game", "NewPhaseDesc", "A node containing data relevant to a Phase of the game.");
#if WITH_EDITORONLY_DATA
	ContextMenuName = NSLOCTEXT("Game", "GamePhase", "Game Phase");
#endif

	PlayerLight.Intensity = 0.025f;
	PlayerLight.AttenuationRadius = 500.0f;
	PlayerLight.bUseTemperature = true;
	PlayerLight.Temperature = 12000.0f;
	PlayerLight.bLightCastShadows = false;
	PlayerLight.bUseInverseSquaredFalloff = true;
	PlayerLight.LightFalloffExponent = 1.0f;
	PlayerLight.SpecularScale = 0.0f;
}

TSet<FGameplayTag> UToroGamePhaseNode::GetContentTags() const
{
	return TSet(UnlockContent.GetGameplayTagArray());
}

TSet<TSoftObjectPtr<UWorld>> UToroGamePhaseNode::GetLevels() const
{
	TSet<TSoftObjectPtr<UWorld>> Result;
	Result.Reserve(Levels.Num() + 1);
	if (!MainLevel.IsNull()) Result.Add(MainLevel);
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Pair : Levels)
	{
		if (!Pair.Key.IsNull()) Result.Add(Pair.Key);
	}
	return Result;
}

TSoftObjectPtr<UTexture2D> UToroGamePhaseNode::GetBackground() const
{
	if (bSimpleLoading || Backgrounds.IsEmpty())
	{
		return nullptr;
	}
	if (Backgrounds.Num() == 1)
	{
		return Backgrounds[0];
	}
	return Backgrounds[FMath::RandRange(0, Backgrounds.Num() - 1)];
}

void UToroGamePhaseNode::TeleportPlayer() const
{
	if (TeleportPoint.LoadSynchronous())
	{
		TeleportPoint->TeleportPlayer();
	}
}

void UToroGamePhaseNode::ApplyPlayerSettings(AToroPlayerCharacter* PlayerChar) const
{
	PlayerChar->SetLightSettings(PlayerLight);
}

#if WITH_EDITOR
void ShowInventoryError(const UInventoryAsset* Asset, const FText& Expected)
{
	FNotificationInfo Info(FText::Format(
		INVTEXT("\"{0}\" is not a valid {1}"),
		FText::FromString(GetNameSafe(Asset)), Expected)
	);
	Info.ExpireDuration = 1.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}

void UToroGamePhaseNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
	if (Quest.LoadSynchronous() == UQuest::StaticClass()) Quest.Reset();
	
	PlayerLight.bUseInverseSquaredFalloff = true;

	for (auto It = Archives.CreateIterator(); It; ++It)
	{
		const UInventoryAsset* Asset = It->LoadSynchronous();
		if (Asset && !Asset->IsValidArchive())
		{
			It->Reset();
			ShowInventoryError(Asset, INVTEXT("Archive"));
		}
	}
	for (auto It = Items.CreateIterator(); It; ++It)
	{
		const UInventoryAsset* Asset = It.Key().LoadSynchronous();
		if (Asset && Asset->AssetType != EInvAssetType::Item)
		{
			It.Key().Reset();
			ShowInventoryError(Asset, INVTEXT("Item"));
		}
		else if (It.Value() == 0)
		{
			It.Value() = 1;
		}
	}
}
#endif