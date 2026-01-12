// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "GamePhase/GamePhaseManager.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "Narrative/NarrativeManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "MusicSystem/WorldMusicManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "PostProcessing/MasterPostProcess.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Inventory/InventoryManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "SaveSystem/ToroGameSave.h"
#include "Helpers/LatentInfo.h"
#include "ToroRuntime.h"

namespace MenuThemeTags
{
	DEFINE_GAMEPLAY_TAG(MenuTheme)
	DEFINE_GAMEPLAY_TAG_CHILD(MenuTheme, Default)
	DEFINE_GAMEPLAY_TAG_CHILD(MenuTheme, Ending)
}

UGamePhaseManager::UGamePhaseManager(): bLoading(false), PhaseTime(0.0f), UnloadTasks(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}

void UGamePhaseManager::LoadSequence()
{
	if (!IsValidManager() || bLoading) return;
	if (const UToroGameSave* Save = SaveManager->FindOrAddSave<UToroGameSave>())
	{
		Sequence = Graph->ValidateSequence(Save->Sequence);
		ChangePhase(Graph->GetLeafInSequence<UToroGamePhaseNode>(Sequence, true));
	}
}

void UGamePhaseManager::StepSequence(const uint8 InIndex)
{
	if (!IsValidManager() || bLoading) return;
	if (UToroGameSave* Save = SaveManager->FindOrAddSave<UToroGameSave>())
	{
		Sequence.Add(InIndex);
		Sequence = Graph->ValidateSequence(Sequence);
		ChangePhase(Graph->GetLeafInSequence<UToroGamePhaseNode>(Sequence, true));
		Save->Sequence = Sequence;
	}
}

void UGamePhaseManager::ChangePhase(UToroGamePhaseNode* NewPhase)
{
	if (!IsValidManager() || bLoading || !NewPhase || NewPhase == ThisPhase
		|| !Graph->AllNodes.Contains(NewPhase)) return;

	UToroGameSave* Save = SaveManager->FindOrAddSave<UToroGameSave>();
	if (!Save) return;

	bLoading = true;
	PlayerChar->AddLockTag(PlayerLockTags::TAG_Loading);
	if (AToroPlayerController* PC = PlayerChar->GetPlayerController<AToroPlayerController>())
	{
		PC->EnterCinematic(GetOwner());
	}

	// Don't need to worry about OldPhase getting GC'd since it lives in Graph which is keeping it loaded
	UToroGamePhaseNode* OldPhase = ThisPhase;
	ThisPhase = NewPhase;

	UnloadTasks = 0;
	LoadLevels = ThisPhase->GetLevels();
	if (OldPhase)
	{
		UnloadLevels = OldPhase->GetLevels().Difference(LoadLevels);
		Narrative->ForgetQuest(OldPhase->Quest.LoadSynchronous());
		Save->PlayTime += PhaseTime;
		PhaseTime = 0.0f;
	}
	else
	{
		UnloadLevels.Empty();
	}

	FadeToBlack();
	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, OldPhase = OldPhase]()
	{
		if (ULoadingScreenWidget* Widget = GetLoadingWidget())
		{
			Widget->ShowWidget(ThisPhase->bSimpleLoading);
		}

		OldPhase ? Inventory->PushToSave() : Inventory->PullFromSave();
		Inventory->EnsureInventory(ThisPhase->Archives, ThisPhase->Items);
		PlayerChar->Teleport(FVector::ZeroVector, FRotator::ZeroRotator);
		PostProcessing->SetUDSSettings(ThisPhase->SkyWeather);

		for (const TSoftObjectPtr<UWorld>& Level : UnloadLevels)
		{
			UnloadLevel(Level);
		}
		for (const TSoftObjectPtr<UWorld>& Level : LoadLevels)
		{
			LoadLevel(Level);
		}
	}, 0.6f, false);

	if (UToroGlobalSave* GlobalSave = SaveManager->FindOrAddSave<UToroGlobalSave>(0))
	{
		GlobalSave->Themes.AddUnique(ThisPhase->UnlockTheme);
		GlobalSave->Content.Append(ThisPhase->GetContentTags());
		GlobalSave->SaveObject(nullptr);
	}
}

void UGamePhaseManager::SetSequence(const TArray<uint8>& InSequence)
{
	if (!IsValidManager() || bLoading) return;
	Sequence = Graph->ValidateSequence(InSequence.IsEmpty() ? TArray<uint8>{0} : InSequence);
	ChangePhase(Graph->GetLeafInSequence<UToroGamePhaseNode>(Sequence, true));
}

ULoadingScreenWidget* UGamePhaseManager::GetLoadingWidget()
{
	if (!LoadingWidget)
	{
		LoadingWidget = AToroWidgetManager::GetWidget<ULoadingScreenWidget>(this);
	}
	return LoadingWidget;
}

bool UGamePhaseManager::IsValidManager() const
{
	return Graph && PlayerChar && SaveManager;
}

void UGamePhaseManager::UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel)
{
	if (!InLevel.IsNull())
	{
		UnloadTasks++;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, InLevel,
				LatentInfo::Make(TEXT("OnUnloadLevel"), this),false);
	}
}

void UGamePhaseManager::LoadLevel(const TSoftObjectPtr<UWorld>& InLevel)
{
	if (!InLevel.IsNull() && ThisPhase)
	{
		const bool bMainLevel = InLevel == ThisPhase->MainLevel;
		const bool bVisible = bMainLevel || ThisPhase->Levels.FindRef(InLevel);
		const FName MapName(*FPackageName::ObjectPathToPackageName(InLevel.ToString()));

		ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, MapName);
		if (Level && Level->IsLevelLoaded())
		{
			Level->SetShouldBeVisible(bVisible);
			if (bMainLevel)
			{
				OnMainLevelLoaded();
			}
			return;
		}

		UGameplayStatics::LoadStreamLevel(this, MapName, bVisible, false,
			bMainLevel ? LatentInfo::Make(TEXT("OnMainLevelLoaded"), this) : LatentInfo::Make());
	}
}

void UGamePhaseManager::OnUnloadLevel()
{
	if (UnloadTasks > 0) UnloadTasks--;
	if (UnloadTasks == 0)
	{
		UToroGeneralUtils::ForceGarbageCollection();
	}
}

void UGamePhaseManager::FadeToBlack() const
{
	UToroShortcutLibrary::StartCameraFade(this, 0.0, 1.0,
		0.5f, FLinearColor::Black, true, true);
}

void UGamePhaseManager::FadeFromBlack() const
{
	UToroShortcutLibrary::StartCameraFade(this, 1.0, 0.0,
		0.5f, FLinearColor::Black, true, false);
}

void UGamePhaseManager::OnMainLevelLoaded()
{
	if (!ThisPhase)
	{
		return;
	}

	ThisPhase->TeleportPlayer();
	ThisPhase->ApplyPlayerSettings(PlayerChar);
	MusicManager->ChangeMainTheme(ThisPhase->Soundtrack.LoadSynchronous());

	if (ULoadingScreenWidget* Widget = GetLoadingWidget())
	{
		Widget->PopWidget();
	}

	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this]()
	{
		bLoading = false;
		Narrative->BeginQuest(ThisPhase->Quest.LoadSynchronous());
		PlayerChar->RemoveLockTag(PlayerLockTags::TAG_Loading);
		if (AToroPlayerController* PC = PlayerChar->GetPlayerController<AToroPlayerController>())
		{
			PC->ExitCinematic();
		}
		UToroShortcutLibrary::CallRemoteEvent(this, ThisPhase->PostLoadEvent);
		FadeFromBlack();
	}, 1.0f, false);
}

void UGamePhaseManager::BeginPlay()
{
	Super::BeginPlay();
	UToroSettings* Settings = UToroSettings::Get();
	if (Settings && Settings->IsOnMap(this, EToroMapType::Gameplay))
	{
		if (Graph = Settings->PhaseGraph.LoadSynchronous(); Graph)
		{
			SetComponentTickEnabled(true);
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				Narrative = UNarrativeManager::Get(this);
				Inventory = UInventoryManager::Get(this);
				SaveManager = UToroSaveManager::Get(this);
				MusicManager = UWorldMusicManager::Get(this);
				PlayerChar = AToroPlayerCharacter::Get(this);
				PostProcessing = AMasterPostProcess::Get(this);
			});
		}
	}
}

void UGamePhaseManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bLoading) PhaseTime += DeltaTime;
}
