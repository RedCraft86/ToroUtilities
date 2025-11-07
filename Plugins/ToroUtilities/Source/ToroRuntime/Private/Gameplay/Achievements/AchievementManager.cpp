// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Achievements/AchievementManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "ToroRuntime.h"

void UAchievementManager::Grant(const FGameplayTag Key, const uint8 Value) const
{
	if (UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		const uint8 Progress = FMath::Min(255, Save->Achievements.FindRef(Key) + Value);
		Save->Achievements.Add(Key, Progress);
		Save->SaveObject(nullptr);

		const FAchievementEntry* Entry = Database ? Database->GetEntry(Key) : nullptr;
		if (Entry && Entry->IsValidData() && Entry->Requirement == Progress)
		{
			OnAchievement.Broadcast(*Entry);
		}
	}
}

uint8 UAchievementManager::GetProgress(const FGameplayTag Key) const
{
	if (!AchievementTags::IsValidTag(Key)) return 0;
	if (const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		return Save->Achievements.FindRef(Key);
	}
	return 0;
}

bool UAchievementManager::HasAchievement(const FGameplayTag Key) const
{
	if (const FAchievementEntry* Entry = Database ? Database->GetEntry(Key) : nullptr)
	{
		return GetProgress(Key) >= Entry->Requirement;
	}
	return false;
}

void UAchievementManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
	Database = UToroSettings::Get()->GetDatabase<UAchievementDatabase>();
#if WITH_EDITOR
	if (!Database)
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Achievement Database not found in settings!"))
	}
#endif
}
