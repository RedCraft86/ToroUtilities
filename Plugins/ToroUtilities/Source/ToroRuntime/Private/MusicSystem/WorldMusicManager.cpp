// Copyright (C) RedCraft86. All Rights Reserved.

#include "MusicSystem/WorldMusicManager.h"
#include "ToroRuntime.h"

UWorldMusicManager::UWorldMusicManager(): StateIdx(0), bGamePaused(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetUISound(true);
	bAutoActivate = false;
	bCanPlayMultipleInstances = false;
	bIsMusic = true;
}

void UWorldMusicManager::ChangeMainTheme(UMetaSoundSource* InTheme)
{
	if (InTheme && Sound != InTheme)
	{
		if (bIsPaused)
		{
			Stop();
			SetPaused(false);
			SetSound(InTheme);
			FadeIn(1.0f);
		}
		else
		{
			FadeOut(1.0f, 0.0f);
			GetWorld()->GetTimerManager().ClearTimer(ChangeTimer);
			GetWorld()->GetTimerManager().SetTimer(ChangeTimer, [this, InTheme]()
			{
				SetSound(InTheme);
				FadeIn(1.0f);
			}, 1.1f, false);
		}

		SetThemeState(0);
		SetThemeIntensity(0.0f);
		ClearDipRequests();
		OnThemeChanged.Broadcast();
	}
}

void UWorldMusicManager::SetThemeState(const uint8 State)
{
	if (StateIdx != State)
	{
		StateIdx = State;
		GetParamInterface()->SetIntParameter(TEXT("State"), State);
		OnStateChanged.Broadcast(StateIdx);
	}
}

void UWorldMusicManager::SetThemeIntensity(const uint8 Intensity)
{
	GetParamInterface()->SetIntParameter(TEXT("Intensity"), Intensity);
}

void UWorldMusicManager::AddDipRequest(const UObject* InObject)
{
	if (InObject && !DipRequests.Contains(InObject))
	{
		DipRequests.Add(InObject);
		UpdateDipState();
	}
}

void UWorldMusicManager::RemoveDipRequest(const UObject* InObject)
{
	if (InObject && DipRequests.Remove(InObject) > 0)
	{
		UpdateDipState();
	}
}

void UWorldMusicManager::ClearDipRequests()
{
	DipRequests.Empty();
	UpdateDipState();
}

void UWorldMusicManager::UpdateDipState()
{
	for (auto It = DipRequests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	const bool bDipped = !DipRequests.IsEmpty(); // Pause is not considered for the delegate
	GetParamInterface()->SetBoolParameter(TEXT("Dipped"), bGamePaused || bDipped);
	if (bDipped) OnAudioDipped.Broadcast();
}

void UWorldMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if (UMetaSoundSource* Default = UToroSettings::Get()->DefaultTheme.LoadSynchronous())
	{
		SetSound(Default);
		FadeIn(1.0f);
	}
}

void UWorldMusicManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	const bool bIsGamePaused = GetWorld()->IsPaused();
	if (bGamePaused != bIsGamePaused)
	{
		bGamePaused = bIsGamePaused;
		UpdateDipState();
	}
}

IAudioParameterControllerInterface* UWorldMusicManager::GetParamInterface()
{
	return Cast<IAudioParameterControllerInterface>(this);
}
