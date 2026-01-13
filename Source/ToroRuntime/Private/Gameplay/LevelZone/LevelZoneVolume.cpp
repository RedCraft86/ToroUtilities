// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "LevelZone/LevelZoneVolume.h"
#include "LevelZone/LevelZoneManager.h"
#include "Actors/ToroCharacter.h"

ALevelZoneVolume::ALevelZoneVolume(): ThemeIntensity(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 0.1f;

	ActionManager = CreateDefaultSubobject<UWorldActionManager>("ActionManager");
	ActionManager->bAutoConstruction = false;

#if WITH_EDITORONLY_DATA
	// bDisplayShadedVolume = true;
	bRunConstructionScriptOnDrag = true;
#endif
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		MusicManager = UWorldMusicManager::Get(this);
	});
}

void ALevelZoneVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsEnabled())
	{
		const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor);
		ULevelZoneManager* Manager = ULevelZoneManager::Get(this);
		if (CharacterTags::IsValidTag(CharTag) && Manager)
		{
			Manager->AssignCharacterToZone(CharTag, ZoneTag);
		}
		if (CharTag == CharacterTags::TAG_Player.GetTag())
		{
			ActionManager->SetActions(ActionsEnter, false);
			ActionManager->RunActions();
		}
		if (MusicManager)
		{
			MusicManager->SetThemeIntensity(ThemeIntensity);
		}
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (IsEnabled() && CharacterTags::IsPlayer(OtherActor))
	{
		ActionManager->SetActions(ActionsExit, false);
		ActionManager->RunActions();
	}
}

#if WITH_EDITOR
void ALevelZoneVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(ActionsEnter, false);
	ActionManager->AppendActions(ActionsExit, true);

	// bDisplayShadedVolume = true;
	BrushColor = (FLinearColor::MakeRandomColor().Desaturate(0.4f) * 0.5f).ToFColor(true);
	UPDATE_VISUAL_ICON(TEXT("/ToroUtilities/Icons/LevelZoneVolume.LevelZoneVolume"));
}
#endif