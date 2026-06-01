// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroGameMode.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroPlayerHUD.h"
#include "Framework/ToroGameState.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	GameStateClass = AToroGameState::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	HUDClass = AToroPlayerHUD::StaticClass();

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void AToroGameMode::BeginPlay()
{
	Super::BeginPlay();
	FWorldGetter::SetWorld(GetWorld());
	GameInstance = GetGameInstance<UToroGameInstance>();
	if (GameInstance.IsValid())
	{
		GameInstance->OnWorldBeginPlay(GetWorld());
	}
}

void AToroGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameInstance.IsValid())
	{
		GameInstance->OnWorldTick(GetWorld(), DeltaTime);
	}
}
