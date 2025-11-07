// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/MiscActions.h"
#include "Gameplay/GamePhase/GamePhaseManager.h"
#include "Gameplay/Tutorials/TutorialManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/LatentInfo.h"

void FWTMisc_LevelStreaming::RunAction()
{
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Level : Load)
	{
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorldContext(), Level.Key,
			Level.Value, false, LatentInfo::Make());
	}
	for (const TSoftObjectPtr<UWorld>& Level : Unload)
	{
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorldContext(),
			Level, LatentInfo::Make(), false);
	}
}

void FWTMisc_LevelStreaming::OnBeginPlay()
{
	for (auto It = Load.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull()) It.RemoveCurrent();
	}
	for (auto It = Unload.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}

void FWTMisc_StepPhase::RunAction()
{
	if (UGamePhaseManager* Manager = UGamePhaseManager::Get(GetWorldContext()))
	{
		Manager->StepSequence(Index);
	}
}

void FWTMisc_Content::RunAction()
{
	Content.Remove(FGameplayTag::EmptyTag);
	UToroSaveManager* SaveManager = UToroSaveManager::Get(GetWorldContext());
	if (UToroGlobalSave* Save = SaveManager->FindOrAddSave<UToroGlobalSave>(0))
	{
		Save->Content.Append(Content);
		Save->SaveObject(nullptr);
	}
}

void FWTMisc_Tutorial::RunAction()
{
	if (UTutorialManager* Manager = UTutorialManager::Get(GetWorldContext()))
	{
		Manager->QueueTutorials(Tutorials);
	}
}

void FWTMisc_Subtitle::RunAction()
{
	USubtitleWidget::QueueSubtitles(GetWorldContext(), Subtitles);
}

void FWTMisc_Notice::RunAction()
{
	bTitle ? UNoticeWidget::QueueTitle(GetWorldContext(), Message, bImmediate)
		: UNoticeWidget::QueueNotice(GetWorldContext(), Message, bImmediate);
}
