// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroCameraLibrary.generated.h"

/**
 * A utility library providing functions for the user camera.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroCameraLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * In the editor, gets the view location and rotation of the viewport camera; in runtime, gets that of the player.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 * @note Cannot choose between different viewports, will use the main, controlled one.
	 */
	UFUNCTION(BlueprintCallable, Category = Camera, meta = (WorldContext = ContextObject, Keywords = "view point"))
		static FTransform GetViewTransform(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Immediately clears any active camera fade on the player's camera manager.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool StopCameraFade(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Sets a static camera fade alpha without an animation over time.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool SetCameraFade(const UObject* ContextObject, const FLinearColor Color = FLinearColor::Black,
			const float Alpha = 1.0f, const bool bFadeAudio = true, const int32 PlayerIndex = 0);

	/**
	 * Performs a camera fade over time.
	 * @param Color The color of the overlay.
	 * @param Duration How long the fade takes in seconds.
	 * @param FromAlpha Starting opacity (0 = transparent).
	 * @param ToAlpha Target opacity (1 = opaque).
	 * @param bFadeAudio If true, the game audio volume will follow the fade alpha.
	 * @param bHoldAtEnd If true, the fade remains on screen after completion.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 * @param bSuccess True if camera fade was initiated.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject,
		Keywords = "fade camera", AdvancedDisplay = "FromAlpha, ToAlpha, bFadeAudio, bHoldAtEnd"))
		static FVoidCoroutine StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			const FLinearColor Color = FLinearColor::Black, const float Duration = 1.0f, const float FromAlpha = 0.0f,
			const float ToAlpha = 1.0f, const bool bFadeAudio = true, const bool bHoldAtEnd = true, const int32 PlayerIndex = 0);

	/**
	 * Gets the player's current view target actor.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 * @return The player's current view actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = ContextObject, Keywords = "get view camera"))
		static AActor* GetPlayerViewTarget(const UObject* ContextObject, const int32 PlayerIndex = 0);

	/**
	 * Sets the player's view to a new target without blending over time.
	 * @param NewTarget The Actor to set as a new view target.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = ContextObject, Keywords = "set view camera"))
		static bool SetPlayerViewTarget(const UObject* ContextObject, AActor* NewTarget, const int32 PlayerIndex = 0);

	/**
	 * Blend the player's view to a new Target Actor over time.
	 * @param NewTarget The Actor to set as a new view target.
	 * @param Duration Duration of the transition.
	 * @param BlendFunc The interpolation curve (Linear, Cubic, etc.) of the blend.
	 * @param BlendExp Exponent for EaseIn/Out functions.
	 * @param bLockOutgoing If true, stop the old view target from moving and modifying the blend during the process.
	 * @param PlayerIndex Index in the player controller list. Will most likely just remain 0.
	 * @param bSuccess True if camera blend was initiated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject,
		DefaultToSelf = NewTarget, Keywords = "blend view camera", AdvancedDisplay = "BlendFunc, BlendExp, bLockOutgoing"))
		static FVoidCoroutine BlendPlayerViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			AActor* NewTarget, const float Duration = 1.0f, const EViewTargetBlendFunction BlendFunc = VTBlend_Linear,
			const float BlendExp = 0.0f, const bool bLockOutgoing = false, const int32 PlayerIndex = 0);
};
