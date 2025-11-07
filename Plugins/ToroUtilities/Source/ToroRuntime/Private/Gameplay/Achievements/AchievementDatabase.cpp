// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Achievements/AchievementDatabase.h"
#include "ToroRuntime.h"

namespace AchievementTags
{
	DEFINE_GAMEPLAY_TAG(Achievement)
}

const FAchievementEntry* UAchievementDatabase::GetEntry(const FGameplayTag& Key) const
{
	return AchievementTags::IsValidTag(Key) ? Entries.Find(Key) : nullptr;
}

#if WITH_EDITOR
FIntPoint UAchievementDatabase::GetDataResults() const
{
	int32 Valid = 0;
	for (const TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (AchievementTags::IsValidTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Valid++;
		}
	}
	return FIntPoint(Entries.Num(), Valid);
}

void UAchievementDatabase::UpdateData()
{
	for (TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (!AchievementTags::IsValidTag(Entry.Key))
		{
			Entry.Value.Label = TEXT("ERROR: Invalid Tag/Key");
		}
		else if (!Entry.Value.IsValidData())
		{
			Entry.Value.Label = TEXT("ERROR: Invalid Data/Entry");
		}
		else
		{
			Entry.Value.Label = Entry.Value.Name.ToString();
		}
	}
}
#endif