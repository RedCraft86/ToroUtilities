// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Libraries/ToroCameraLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"
#if WITH_EDITOR
#include "Editor.h"
#endif

FTransform UToroCameraLibrary::GetViewTransform(const UObject* ContextObject, const int32 PlayerIndex)
{
	static TMap<int32, TFrameValue<FTransform>> IdxToTransforms;

#if WITH_EDITOR
	TFrameValue<FTransform>& TransformCache = IdxToTransforms.FindOrAdd(FApp::IsGame() ? PlayerIndex : 0);
#else
	TFrameValue<FTransform>& TransformCache = IdxToTransforms.FindOrAdd(PlayerIndex);
#endif

	if (TransformCache.IsSet())
	{
		return TransformCache.GetValue();
	}

	TransformCache = FTransform::Identity;

#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (const FViewport* ActiveVP = GEditor->GetActiveViewport())
		{
			if (const FEditorViewportClient* ActiveVPC = static_cast<FEditorViewportClient*>(ActiveVP->GetClient()))
			{
				TransformCache = FTransform(ActiveVPC->GetViewRotation(), ActiveVPC->GetViewLocation(), FVector::OneVector);
			}
		}

		return TransformCache.GetValue();
	}
#endif

	if (const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		TransformCache = FTransform(PCM->GetCameraRotation(), PCM->GetCameraLocation(), FVector::OneVector);
	}

	return TransformCache.GetValue();
}

bool UToroCameraLibrary::StopCameraFade(const UObject* ContextObject, const int32 PlayerIndex)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		PCM->StopCameraFade();
		return true;
	}

	return false;
}

bool UToroCameraLibrary::SetCameraFade(const UObject* ContextObject, const FLinearColor Color,
	const float Alpha, const bool bFadeAudio, const int32 PlayerIndex)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		PCM->SetManualCameraFade(Alpha, Color, bFadeAudio);
		return true;
	}

	return false;
}

FVoidCoroutine UToroCameraLibrary::StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	bool& bSuccess, const FLinearColor Color, const float Duration, const float FromAlpha, const float ToAlpha,
	const bool bFadeAudio, const bool bHoldAtEnd, const int32 PlayerIndex)
{
	bSuccess = false;
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		bSuccess = true;
		PCM->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bFadeAudio, bHoldAtEnd);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}

AActor* UToroCameraLibrary::GetPlayerViewTarget(const UObject* ContextObject, const int32 PlayerIndex)
{
	const APlayerController* PC = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex);
	return PC ? PC->GetViewTarget() : nullptr;
}

bool UToroCameraLibrary::SetPlayerViewTarget(const UObject* ContextObject, AActor* NewTarget, const int32 PlayerIndex)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		PC->SetViewTarget(NewTarget);
		return true;
	}

	return false;
}

FVoidCoroutine UToroCameraLibrary::BlendPlayerViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	bool& bSuccess, AActor* NewTarget, const float Duration, const EViewTargetBlendFunction BlendFunc,
	const float BlendExp, const bool bLockOutgoing, const int32 PlayerIndex)
{
	bSuccess = false;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		bSuccess = true;
		PC->SetViewTargetWithBlend(NewTarget, Duration, BlendFunc, BlendExp, bLockOutgoing);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}
