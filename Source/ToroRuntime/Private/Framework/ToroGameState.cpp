// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroGameState.h"
#include "GamePhase/GamePhaseManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "SaveSystem/ToroGameSave.h"

AToroGameState::AToroGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GamePhase = CreateDefaultSubobject<UGamePhaseManager>("GamePhase");
}

float AToroGameState::GetTotalPlayProgress() const
{
	UToroSaveManager* SM = UToroSaveManager::Get(this);
	if (const UToroGameSave* Save = SM ? SM->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		const int32 Total = Save->Progress.Num();
		if (Total == 0)
		{
			return 0.0f;
		}

		int32 Finished = 0;
		for (const TPair<FGuid, bool>& Entry : Save->Progress)
		{
			if (Entry.Value) Finished++;
		}

		return static_cast<float>(Finished) / static_cast<float>(Total);
	}
	return -1.0f;
}

float AToroGameState::GetTotalPlayTime() const
{
	UToroSaveManager* SM = UToroSaveManager::Get(this);
	if (const UToroGameSave* Save = SM ? SM->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		return Save->PlayTime;
	}
	return -1.0f;
}
