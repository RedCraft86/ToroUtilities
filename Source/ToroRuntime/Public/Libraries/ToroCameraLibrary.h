// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroCameraLibrary.generated.h"

/**
 * Provides camera related functions.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroCameraLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Gets the player camera transform, or the active editor viewport transform outside gameplay.
	 * @param ContextObject Object used to find the world.
	 * @param PlayerIndex Local player index.
	 * @return Valid camera transform or identity when no camera or viewport is available.
	 */
	UFUNCTION(BlueprintCallable, Category = Camera, meta = (WorldContext = ContextObject, Keywords = "view point"))
		static FTransform GetViewTransform(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Stops the current camera fade.
	 * @param ContextObject Object used to find the world.
	 * @param PlayerIndex Local player index.
	 * @return Whether a player camera manager was found.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool StopCameraFade(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Sets a manual camera fade.
	 * @param ContextObject Object used to find the world.
	 * @param Color Fade color.
	 * @param Alpha Fade opacity.
	 * @param bFadeAudio Whether to fade audio as well.
	 * @param PlayerIndex Local player index.
	 * @return Whether a player camera manager was found.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool SetCameraFade(const UObject* ContextObject, const FLinearColor Color = FLinearColor::Black,
			const float Alpha = 1.0f, const bool bFadeAudio = true, const int32 PlayerIndex = 0);

	/**
	 * Starts a camera fade and completes after its duration.
	 * @param ContextObject Object used to find the world.
	 * @param bSuccess Receives whether a player camera manager was found.
	 * @param Color Fade color.
	 * @param Duration Fade duration in seconds.
	 * @param FromAlpha Initial fade opacity.
	 * @param ToAlpha Final fade opacity.
	 * @param bFadeAudio Whether to fade audio as well.
	 * @param bHoldAtEnd Whether to hold the final fade opacity.
	 * @param PlayerIndex Local player index.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject,
		Keywords = "fade camera", AdvancedDisplay = "FromAlpha, ToAlpha, bFadeAudio, bHoldAtEnd"))
		static FVoidCoroutine StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			const FLinearColor Color = FLinearColor::Black, const float Duration = 1.0f, const float FromAlpha = 0.0f,
			const float ToAlpha = 1.0f, const bool bFadeAudio = true, const bool bHoldAtEnd = true, const int32 PlayerIndex = 0);

	/**
	 * Gets the actor currently used as the player view target.
	 * @param ContextObject Object used to find the world.
	 * @param PlayerIndex Local player index.
	 * @return The current view target, or nullptr if no player controller exists.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = ContextObject, Keywords = "get view camera"))
		static AActor* GetPlayerViewTarget(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Changes the player view target.
	 * @param ContextObject Object used to find the world.
	 * @param NewTarget Actor to use as the view target.
	 * @param PlayerIndex Local player index.
	 * @return Whether a player controller was found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = ContextObject, Keywords = "set view camera"))
		static bool SetPlayerViewTarget(const UObject* ContextObject, AActor* NewTarget, const int32 PlayerIndex = 0);

	/**
	 * Blends to a new player view target and completes after the blend duration.
	 * @param ContextObject Object used to find the world.
	 * @param bSuccess Receives whether a player controller was found.
	 * @param NewTarget Actor to blend to.
	 * @param Duration Blend duration in seconds.
	 * @param BlendFunc Blend interpolation function.
	 * @param BlendExp Exponent used by applicable blend functions.
	 * @param bLockOutgoing Whether to lock the outgoing view during the blend.
	 * @param PlayerIndex Local player index.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject,
		DefaultToSelf = NewTarget, Keywords = "blend view camera", AdvancedDisplay = "BlendFunc, BlendExp, bLockOutgoing"))
		static FVoidCoroutine BlendPlayerViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			AActor* NewTarget, const float Duration = 1.0f, const EViewTargetBlendFunction BlendFunc = VTBlend_Linear,
			const float BlendExp = 0.0f, const bool bLockOutgoing = false, const int32 PlayerIndex = 0);
};
