// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameState.h"
#include "GamePhase/GamePhaseManager.h"

AToroGameState::AToroGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GamePhase = CreateDefaultSubobject<UGamePhaseManager>("GamePhase");
}
