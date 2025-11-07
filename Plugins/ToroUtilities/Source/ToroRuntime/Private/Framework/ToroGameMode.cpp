// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroGameState.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "UserInterface/ToroWidgetManager.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	GameStateClass = AToroGameState::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	HUDClass = AToroWidgetManager::StaticClass();

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AToroGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (GameInstance = UToroGameInstance::Get(this); GameInstance)
	{
		GameInstance->OnWorldBeginPlay(GetWorld());
	}
}

void AToroGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameInstance)
	{
		GameInstance->OnWorldTick(DeltaTime);
	}
}
