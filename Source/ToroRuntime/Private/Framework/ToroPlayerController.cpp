// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCameraManager.h"

AToroPlayerController::AToroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	PlayerCameraManagerClass = AToroPlayerCameraManager::StaticClass();

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void AToroPlayerController::SetInputConfig(const FGameInputConfig& InConfig)
{
	InputConfig = InConfig;
	SetShowMouseCursor(InputConfig.ShouldShowMouse());
	SetInputMode(InputConfig.GetInputMode());
	FlushPressedKeys();
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputConfig(FGameInputConfig::Default);
}
