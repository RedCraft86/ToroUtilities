// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DataAssets/ToroDatabase.h"

bool UToroDatabase::DoesKeyExist(const FGameplayTag& Key) const
{
	return Key.IsValid() && Entries.Contains(Key);
}

FInstancedStruct UToroDatabase::GetValue(const FGameplayTag& Key) const
{
	if (!DoesKeyExist(Key))
	{
		return FInstancedStruct();
	}

	const FInstancedStruct* FoundValue = Entries.Find(Key);
	return FoundValue->IsValid() ? *FoundValue : FInstancedStruct();
}

bool UToroDatabase::IsValidKey(const FGameplayTag& Key) const
{
	return Key.IsValid() && Key != RootTag && Key.MatchesTag(RootTag);
}

UToroDatabase::UToroDatabase(const FGameplayTag& TagType, const UScriptStruct* StructType)
	: RootTag(TagType), RootStruct(StructType)
{
	ensureAlwaysMsgf(StructType && StructType->IsChildOf<FToroDatabaseEntry>(), 
		TEXT("StructType (%s) must derive from FToroDatabaseEntry"), *GetNameSafe(StructType));
}

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
		if (!IsValidKey(Entry.Key))
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
	if (RootStruct && RootTag.IsValid())
	{
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroDatabase, Entries))
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
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif