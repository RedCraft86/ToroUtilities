// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroShortcutsLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"

bool UToroShortcutsLibrary::ClearCameraFade(const UObject* ContextObject)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), 0))
	{
		PCM->StopCameraFade();
		return true;
	}
	return false;
}

bool UToroShortcutsLibrary::SetCameraFade(const UObject* ContextObject, const FLinearColor Color, const float Alpha, const bool bFadeAudio)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), 0))
	{
		PCM->SetManualCameraFade(Alpha, Color, bFadeAudio);
		return true;
	}
	return false;
}

FVoidCoroutine UToroShortcutsLibrary::StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
	const FLinearColor Color, const float Duration, const float FromAlpha, const float ToAlpha, 
	const bool bFadeAudio, const bool bHoldAtEnd)
{
	bSuccess = false;
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), 0))
	{
		PCM->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bFadeAudio, bHoldAtEnd);
		co_await UE5Coro::Latent::Seconds(Duration);
		bSuccess = true;
	}
}

FVoidCoroutine UToroShortcutsLibrary::SetViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	bool& bSuccess, AActor* NewTarget, const float BlendTime, const EViewTargetBlendFunction BlendFunc,
	const float BlendExp, const bool bLockOutgoing)
{
	bSuccess = false;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), 0))
	{
		PC->SetViewTargetWithBlend(NewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
		co_await UE5Coro::Latent::Seconds(BlendTime);
		bSuccess = true;
	}
}
