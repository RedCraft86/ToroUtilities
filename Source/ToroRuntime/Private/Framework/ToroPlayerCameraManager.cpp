// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroPlayerCameraManager.h"

AToroPlayerCameraManager::AToroPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void AToroPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Ensure each map starts on a black screen for any deferred loading
	SetManualCameraFade(1.0f, FLinearColor::Black, true);

	// Fixes MotionBlur and AA issues when camera is moved while paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Allows photomode-like systems to move the camera even when game is paused
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
