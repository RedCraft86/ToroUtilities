// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Cutscenes/ToroCutsceneActor.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "LevelSequencePlayer.h"

AToroCutsceneActor::AToroCutsceneActor(const FObjectInitializer& Init)
	: Super(Init), bSkippable(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetCanBeDamaged(false);
}

void AToroCutsceneActor::PlayAfterFade(const float FadeTime)
{
	if (!GetWorldTimerManager().IsTimerActive(FadedPlayHandle))
	{
		UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, FadeTime);
		GetWorldTimerManager().SetTimer(FadedPlayHandle, [this]()
		{
			FadedPlayHandle.Invalidate();
			PlayCutscene();
		}, FadeTime + UE_SMALL_NUMBER, false);
	}
}

void AToroCutsceneActor::PlayCutscene()
{
	if (!GetWorldTimerManager().IsTimerActive(FadedPlayHandle))
	{
		UToroShortcutLibrary::ClearCameraFade(this);
		GetSequencePlayer()->Play();
		LockPlayer();
	}
}

void AToroCutsceneActor::SkipCutscene()
{
	GetWorldTimerManager().ClearTimer(FadedPlayHandle);
	GetSequencePlayer()->GoToEndAndStop();
	UnlockPlayer();
}

UCutsceneWidget* AToroCutsceneActor::GetCutsceneWidget()
{
	if (!CutsceneWidget)
	{
		return CutsceneWidget = AToroWidgetManager::GetWidget<UCutsceneWidget>(this);
	}
	return CutsceneWidget;
}

void AToroCutsceneActor::OnFinished()
{
	UnlockPlayer();
	OnFinishedEvent.Broadcast();
	if (UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		Save->Cutscenes.Add(CutsceneGuid);
		Save->SaveObject(nullptr);
	}
}

void AToroCutsceneActor::LockPlayer()
{
	if (AToroPlayerController* Player = AToroPlayerController::Get(this))
	{
		Player->EnterCinematic(this);
	}

	if (!bSkippable) return;
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	if (!Save || Save->Cutscenes.Contains(CutsceneGuid))
	{
		if (UCutsceneWidget* Widget = GetCutsceneWidget())
		{
			Widget->ShowWidget(this);
		}
	}
}

void AToroCutsceneActor::UnlockPlayer()
{
	if (AToroPlayerController* Player = AToroPlayerController::Get(this))
	{
		Player->ExitCinematic();
	}

	if (!bSkippable) return;
	if (UCutsceneWidget* Widget = GetCutsceneWidget())
	{
		Widget->PopWidget();
	}
}

void AToroCutsceneActor::BeginPlay()
{
	Super::BeginPlay();
	GetSequencePlayer()->OnNativeFinished.BindUObject(this, &AToroCutsceneActor::OnFinished);
}

void AToroCutsceneActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!CutsceneGuid.IsValid()) CutsceneGuid = FGuid::NewGuid();
}
