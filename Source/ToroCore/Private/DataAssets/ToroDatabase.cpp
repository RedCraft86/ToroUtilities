// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "DataAssets/ToroDatabase.h"

#if WITH_EDITOR
void UToroDatabase::ValidateData()
{
	Super::ValidateData();

	if (!RootStruct)
	{
		AppendIssue("RootStruct is null: Database was not inherited properly or may be corrupted.");
	}

	if (!RootTag.IsValid())
	{
		AppendIssue("RootTag is invalid: Database was not inherited properly or may be corrupted.");
	}

	const FString Spacing(TEXT("  └  "));
	TPair<TArray<FString>, TArray<FString>> IssueKeys;
	for (const TPair<FToroDatabaseKey, FInstancedStruct>& Entry : Entries)
	{
		if (!IsValidKey(*Entry.Key))
		{
			IssueKeys.Key.Add(Spacing + TEXT("[Invalid Key] ") + Entry.Key.ToString());
		}

		if (!Entry.Value.IsValid())
		{
			IssueKeys.Value.Add(Spacing + TEXT("[Struct Not Set] ") + Entry.Key.ToString());
		}
		else if (Entry.Value.GetScriptStruct() != RootStruct 
			&& !Entry.Value.GetScriptStruct()->IsChildOf(RootStruct))
		{
			IssueKeys.Value.Add(Spacing + TEXT("[Incompatible Type] ") + Entry.Key.ToString());
		}
		else if (const FToroDatabaseEntry* ValuePtr = Entry.Value.GetPtr<FToroDatabaseEntry>())
		{
			if (!ValuePtr->IsValid())
			{
				IssueKeys.Value.Add(Spacing + TEXT("[Invalid Data] ") + Entry.Key.ToString());
			}
		}
	}

	if (!IssueKeys.Key.IsEmpty())
	{
		AppendIssue(FString::Printf(
			TEXT("Issues with KEYS of the following entries:\n%s"),
			*FString::Join(IssueKeys.Key, TEXT("\n"))
		));
	}

	if (!IssueKeys.Value.IsEmpty())
	{
		AppendIssue(FString::Printf(
			TEXT("Issues with VALUES of the following entries:\n%s"),
			*FString::Join(IssueKeys.Value, TEXT("\n"))
		));
	}
}

void UToroDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (RootStruct && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroDatabase, Entries))
	{
		for (TPair<FToroDatabaseKey, FInstancedStruct>& Entry : Entries)
		{
			if (!Entry.Key.IsValid())
			{
				Entry.Key = RootTag;	
			}
			if (!Entry.Value.IsValid())
			{
				Entry.Value.InitializeAs(RootStruct);
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif