// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Tutorials/TutorialDatabase.h"

namespace TutorialTags
{
	DEFINE_GAMEPLAY_TAG(Tutorial)
}

const FTutorialEntry* UTutorialDatabase::GetEntry(const FGameplayTag& Key) const
{
	return TutorialTags::IsValidTag(Key) ? Entries.Find(Key) : nullptr;
}

#if WITH_EDITOR
FIntPoint UTutorialDatabase::GetDataResults() const
{
	int32 Valid = 0;
	for (const TPair<FGameplayTag, FTutorialEntry>& Entry : Entries)
	{
		if (TutorialTags::IsValidTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Valid++;
		}
	}
	return FIntPoint(Entries.Num(), Valid);
}

void UTutorialDatabase::UpdateData()
{
	for (TPair<FGameplayTag, FTutorialEntry>& Entry : Entries)
	{
		if (!TutorialTags::IsValidTag(Entry.Key))
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