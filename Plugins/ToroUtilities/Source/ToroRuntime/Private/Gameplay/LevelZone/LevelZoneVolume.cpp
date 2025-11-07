// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZone/LevelZoneVolume.h"
#include "LevelZone/LevelZoneManager.h"
#include "Components/CullingComponent.h"
#include "Actors/ToroCharacter.h"
#if WITH_EDITOR
#include "EngineUtils.h"
#endif

ALevelZoneVolume::ALevelZoneVolume(): ThemeIntensity(0.0f), CullInvert(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 0.1f;

	ActionManager = CreateDefaultSubobject<UWorldActionManager>("ActionManager");
	ActionManager->bAutoConstruction = false;

#if WITH_EDITORONLY_DATA
	bDisplayShadedVolume = true;
	bRunConstructionScriptOnDrag = true;
#endif
}

#if WITH_EDITOR
void ALevelZoneVolume::FindCullTargets()
{
	for (const TObjectPtr<AActor> Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!UCullingComponent::Get(Actor))
		{
			CullTargets.Remove(Actor);
		}
		else if (!CullFindBounded || EncompassesPoint(Actor->GetActorLocation()))
		{
			if (CullFindTag.IsNone() || Actor->ActorHasTag(CullFindTag))
			{
				CullTargets.Add(Actor);
			}
		}
	}
	
	for (auto It = CullTargets.CreateIterator(); It; ++It)
	{
		if (!It->LoadSynchronous()) It.RemoveCurrent();
	}
}
#endif

void ALevelZoneVolume::UpdateRefCulling()
{
	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (CullInvert) bShouldRender = !bShouldRender;

	for (auto It = CullTargets.CreateIterator(); It; ++It)
	{
		UCullingComponent* Comp = UCullingComponent::Get(It->LoadSynchronous());
		if (Comp && Comp->IsComponentTickEnabled())
		{
			bShouldRender ? Comp->AddRenderRequest(this) : Comp->RemoveRenderRequest(this);
		}
		else
		{
			It.RemoveCurrent();
		}
	}
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		MusicManager = UWorldMusicManager::Get(this);
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	});
}

void ALevelZoneVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CamManager) UpdateRefCulling();
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

	bDisplayShadedVolume = true;
	BrushColor = (FLinearColor::MakeRandomColor().Desaturate(0.4f) * 0.5f).ToFColor(true);
	UPDATE_VISUAL_ICON(TEXT("/ToroUtilities/Icons/LevelZoneVolume.LevelZoneVolume"));
}
#endif