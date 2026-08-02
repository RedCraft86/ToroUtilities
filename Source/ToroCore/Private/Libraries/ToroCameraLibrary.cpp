// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroCameraLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"
#if WITH_EDITOR
#include "LevelEditorViewport.h"
#include "Editor.h"
#endif

FTransform UToroCameraLibrary::GetViewTransform(const UObject* ContextObject, const int32 ViewIndex)
{
	static TMap<int32, TFrameValue<FTransform>> IndexedTransformCache;
	TFrameValue<FTransform>& TransformCache = IndexedTransformCache.FindOrAdd(ViewIndex);
	if (TransformCache.IsSet())
	{
		return TransformCache.GetValue();
	}

	TransformCache = FTransform::Identity;

#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		const TArray<FLevelEditorViewportClient*>& Viewports = GEditor->GetLevelViewportClients();
		if (Viewports.IsValidIndex(ViewIndex) && Viewports[ViewIndex])
		{
			const FLevelEditorViewportClient* ViewportClient = Viewports[ViewIndex];
			TransformCache = FTransform(
				ViewportClient->GetViewRotation(),
				ViewportClient->GetViewLocation(),
				FVector::OneVector
			);
		}
		else if (const FViewport* ActiveVP = GEditor->GetActiveViewport())
		{
			if (const FEditorViewportClient* ActiveVPC = static_cast<FEditorViewportClient*>(ActiveVP->GetClient()))
			{
				TransformCache = FTransform(
					ActiveVPC->GetViewRotation(),
					ActiveVPC->GetViewLocation(),
					FVector::OneVector
				);
			}
		}

		return TransformCache.GetValue();
	}
#endif

	if (const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), ViewIndex))
	{
		TransformCache = FTransform(PCM->GetCameraRotation(), PCM->GetCameraLocation(), FVector::OneVector);
	}

	return TransformCache.GetValue();
}

bool UToroCameraLibrary::StopCameraFade(const UObject* ContextObject, const int32 PlayerIndex)
{
	if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		CamManager->StopCameraFade();
		return true;
	}

	return false;
}

bool UToroCameraLibrary::SetCameraFade(const UObject* ContextObject, const FLinearColor Color,
	const float Alpha, const bool bFadeAudio, const int32 PlayerIndex)
{
	if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		CamManager->SetManualCameraFade(Alpha, Color, bFadeAudio);
		return true;
	}

	return false;
}

FVoidCoroutine UToroCameraLibrary::StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	bool& bSuccess, const FLinearColor Color, const float Duration, const float FromAlpha, const float ToAlpha,
	const bool bFadeAudio, const bool bHoldAtEnd, const int32 PlayerIndex)
{
	bSuccess = false;
	if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		bSuccess = true;
		CamManager->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bFadeAudio, bHoldAtEnd);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}

AActor* UToroCameraLibrary::GetPlayerViewTarget(const UObject* ContextObject, const int32 PlayerIndex)
{
	const APlayerController* PlayerCont = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex);
	return PlayerCont ? PlayerCont->GetViewTarget() : nullptr;
}

bool UToroCameraLibrary::SetPlayerViewTarget(const UObject* ContextObject, AActor* NewTarget, const int32 PlayerIndex)
{
	if (APlayerController* PlayerCont = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		PlayerCont->SetViewTarget(NewTarget);
		return true;
	}

	return false;
}

FVoidCoroutine UToroCameraLibrary::BlendPlayerViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	bool& bSuccess, AActor* NewTarget, const float Duration, const EViewTargetBlendFunction BlendFunc,
	const float BlendExp, const bool bLockOutgoing, const int32 PlayerIndex)
{
	bSuccess = false;
	if (APlayerController* PlayerCont = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIndex))
	{
		bSuccess = true;
		PlayerCont->SetViewTargetWithBlend(NewTarget, Duration, BlendFunc, BlendExp, bLockOutgoing);
		co_await UE5Coro::Latent::Seconds(Duration);
	}

	co_return;
}
