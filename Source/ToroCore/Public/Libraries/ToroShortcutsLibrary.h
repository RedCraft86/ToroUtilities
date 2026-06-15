// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "Engine/LatentActionManager.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroShortcutsLibrary.generated.h"

/**
 * Provides "Shortcut" nodes that combine multiple engine calls or nested common functions 
 * into single, latent actions for smoother Blueprint graphs.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroShortcutsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Immediately clears any active camera fade on the player's camera manager. */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool ClearCameraFade(const UObject* ContextObject);

	/** Sets a static camera fade alpha without an animation duration. */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (WorldContext = ContextObject, Keywords = "fade camera"))
		static bool SetCameraFade(const UObject* ContextObject, const FLinearColor Color, const float Alpha, const bool bFadeAudio);

	/** 
	 * Latent action that performs a camera fade over time.
	 * The Blueprint execution will wait until the fade duration is complete.
	 * 
	 * @param Color       The color of the overlay.
	 * @param Duration    How long the fade takes in seconds.
	 * @param FromAlpha   Starting opacity (0 = transparent).
	 * @param ToAlpha     Target opacity (1 = opaque).
	 * @param bFadeAudio  If true, the game audio volume will follow the fade alpha.
	 * @param bHoldAtEnd  If true, the fade remains on screen after completion.
	 */
	UFUNCTION(BlueprintCallable, Category = CameraFade, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject, 
		Keywords = "fade camera", AdvancedDisplay = "FromAlpha, ToAlpha, bFadeAudio, bHoldAtEnd"))
		static FVoidCoroutine StartCameraFade(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			const FLinearColor Color = FLinearColor::Black, const float Duration = 1.0f, const float FromAlpha = 0.0f, 
			const float ToAlpha = 1.0f, const bool bFadeAudio = true, const bool bHoldAtEnd = true);

	/** 
	 * Latent action to blend the player's view to a new Target Actor.
	 * The Blueprint node will wait until the blend is finished before continuing.
	 * 
	 * @param NewTarget     The Actor to look through.
	 * @param BlendTime     Duration of the transition.
	 * @param BlendFunc     The interpolation curve (Linear, Cubic, etc.).
	 * @param BlendExp      Exponent for EaseIn/Out functions.
	 * @param bLockOutgoing If true, prevents the old view target from moving during the blend.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject, 
		DefaultToSelf = NewTarget, Keywords = "blend camera", AdvancedDisplay = "BlendFunc, BlendExp, bLockOutgoing"))
		static FVoidCoroutine SetViewTarget(FLatentActionInfo LatentInfo, const UObject* ContextObject, bool& bSuccess,
			AActor* NewTarget, const float BlendTime = 1.0f, const EViewTargetBlendFunction BlendFunc = VTBlend_Linear, 
			const float BlendExp = 0.0f, const bool bLockOutgoing = false);
};
