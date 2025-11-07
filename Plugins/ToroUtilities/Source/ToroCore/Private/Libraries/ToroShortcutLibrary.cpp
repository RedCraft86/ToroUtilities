// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroShortcutLibrary.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"

void UToroShortcutLibrary::ClearCameraFade(const UObject* ContextObject)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->StopCameraFade();
	}
}

void UToroShortcutLibrary::SetCameraFade(const UObject* ContextObject, const float Alpha, const FLinearColor Color, const bool bFadeAudio)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->SetManualCameraFade(Alpha, Color, bFadeAudio);
	}
}

void UToroShortcutLibrary::StartCameraFade(UObject* ContextObject, const float FromAlpha, const float ToAlpha,
	const float Duration, const FLinearColor Color, const bool bShouldFadeAudio, const bool bHoldWhenFinished)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bShouldFadeAudio, bHoldWhenFinished);
	}
}

void UToroShortcutLibrary::SetViewTarget(UObject* ContextObject, AActor* NewTarget, const float BlendTime,
	const EViewTargetBlendFunction BlendFunc, const float BlendExp, const bool bLockOutgoing)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, 0))
	{
		PC->SetViewTargetWithBlend(NewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
	}
}

void UToroShortcutLibrary::AddActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.AddUnique(InTag);
	}
}

void UToroShortcutLibrary::RemoveActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.Remove(InTag);
	}
}

void UToroShortcutLibrary::RestartLevel(const UObject* ContextObject, const FString Options)
{
	UGameplayStatics::OpenLevel(ContextObject, *UGameplayStatics::GetCurrentLevelName(ContextObject), true, Options);
}

void UToroShortcutLibrary::CallRemoteEvent(UObject* ContextObject, const FName EventName)
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}

void UToroShortcutLibrary::PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->Play();
	}
}

void UToroShortcutLibrary::ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->SetPlayRate(PlayRate);
		SequencePlayer->PlayReverse();
	}
}

void UToroShortcutLibrary::StopLevelSequence(const ALevelSequenceActor* Target, const ESequenceStopType StopType)
{
	if (ULevelSequencePlayer* SequencePlayer = Target ? Target->GetSequencePlayer() : nullptr)
	{
		switch (StopType)
		{
		case ESequenceStopType::Default:
			SequencePlayer->Stop();
			break;

		case ESequenceStopType::AtCurrentTime:
			SequencePlayer->StopAtCurrentTime();
			break;

		case ESequenceStopType::SkipToEnd:
			SequencePlayer->GoToEndAndStop();
			break;
		}
	}
}

void UToroShortcutLibrary::PlayAmbientSound(const AAmbientSound* Target, const float StartTime)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->Play(StartTime);
	}
}

void UToroShortcutLibrary::StopAmbientSound(const AAmbientSound* Target)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->Stop();
	}
}

void UToroShortcutLibrary::FadeInAmbientSound(const AAmbientSound* Target, float Duration,
	const float TargetLevel, const float StartTime, const EAudioFaderCurve FadeCurve)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->FadeIn(Duration, TargetLevel, StartTime, FadeCurve);
	}
}

void UToroShortcutLibrary::FadeOutAmbientSound(const AAmbientSound* Target,
	const float Duration, const float TargetLevel, const EAudioFaderCurve FadeCurve)
{
	if (Target && Target->GetAudioComponent())
	{
		Target->GetAudioComponent()->FadeOut(Duration, TargetLevel, FadeCurve);
	}
}
