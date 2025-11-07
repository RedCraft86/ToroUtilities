// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerState.h"
#include "Narrative/NarrativeManager.h"
#include "MusicSystem/WorldMusicManager.h"
#include "Achievements/AchievementManager.h"
#include "Inspection/InspectionManager.h"
#include "Inventory/InventoryManager.h"
#include "Tutorials/TutorialManager.h"

AToroPlayerState::AToroPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MusicManager = CreateDefaultSubobject<UWorldMusicManager>("MusicManager");
	MusicManager->SetupAttachment(GetRootComponent());

	Tutorials = CreateDefaultSubobject<UTutorialManager>("Tutorials");
	Inventory = CreateDefaultSubobject<UInventoryManager>("Inventory");
	Inspection = CreateDefaultSubobject<UInspectionManager>("Inspection");
	Narrative = CreateDefaultSubobject<UNarrativeManager>("Narrative");
	Achievements = CreateDefaultSubobject<UAchievementManager>("Achievements");
}
