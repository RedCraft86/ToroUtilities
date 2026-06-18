// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "ProjectLink.h"

class FPLConfigSection final
{
	FORCEINLINE static bool IsArrayKey(const FString& Key)
	{
		return !Key.IsEmpty() && (Key[0] == '+' || Key[0] == '-' || Key[0] == '.' || Key[0] == '!');
	}

	FORCEINLINE static FString NormalizeKey(const FString& Key)
	{
		return IsArrayKey(Key) ? Key.RightChop(1) : Key;
	}

public:

	TArray<TPair<FString, FString>> Entries;

	void AddEntry(const FString& Key, const FString& Value)
	{
		if (IsArrayKey(Key))
		{
			Entries.Add({Key, Value});
			return;
		}

		for (TPair<FString, FString>& Entry : Entries)
		{
			if (Entry.Key == Key)
			{
				Entry.Value = Value;
				return;
			}
		}

		Entries.Add({Key, Value});
	}

	void Append(const FPLConfigSection& Other, const TSet<FString>& Filter)
	{
		if (Filter.IsEmpty())
		{
			UE_LOG(LogProjectLink, Display, TEXT("\t\t\t- Entire Section"));
		}

		for (const TPair<FString, FString>& OEntry : Other.Entries)
		{
			if (Filter.IsEmpty() || Filter.Contains(NormalizeKey(OEntry.Key)))
			{
				if (!Filter.IsEmpty())
				{
					UE_LOG(LogProjectLink, Display, TEXT("\t\t\t- Appending: %s=%s"), *OEntry.Key, *OEntry.Value);
				}

				AddEntry(OEntry.Key, OEntry.Value);
			}
		}
	}
};

class FPLConfigFile final
{
public:

	TSortedMap<FString, FPLConfigSection> Sections;

	void LoadFile(const FString& FilePath)
	{
		Sections.Empty();
		if (FilePath.IsEmpty() || !FilePath.EndsWith(TEXT(".ini")) || !FPaths::FileExists(FilePath))
		{
			return;
		}

		TArray<FString> Lines;
		if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
		{
			FString RecentSection;
			for (FString& Line : Lines)
			{
				Line.TrimStartAndEndInline();
				if (Line.IsEmpty() || Line.StartsWith(TEXT(";")) || Line.StartsWith(TEXT("#")))
				{
					continue;
				}

				if (Line.StartsWith(TEXT("[")) && Line.EndsWith(TEXT("]")))
				{
					RecentSection = Line.Mid(1, Line.Len() - 2);
					Sections.Add(RecentSection);
					continue;
				}

				if (FPLConfigSection* Section = RecentSection.IsEmpty() ? nullptr: Sections.Find(RecentSection))
				{
					FString Key, Value;
					if (!Line.Split(TEXT("="), &Key, &Value))
					{
						Key = Line;
						Value = FString();
					}

					Section->AddEntry(Key, Value);
				}
			}
		}
	}
	
	void SaveFile(const FString& FilePath)
	{
		TArray<FString> Lines;
		for (const TPair<FString, FPLConfigSection>& Section : Sections)
		{
			Lines.Add(FString::Printf(TEXT("[%s]"), *Section.Key));
			for (const TPair<FString, FString>& Entry : Section.Value.Entries)
			{
				Lines.Add(FString::Printf(TEXT("%s=%s"), *Entry.Key, *Entry.Value));
			}
			Lines.Add(FString());
		}

		FFileHelper::SaveStringArrayToFile(Lines, *FilePath);
	}

	void Append(const FPLConfigFile& Other, const TMap<FString, TSet<FString>>& Filter)
	{
		for (const TPair<FString, FPLConfigSection>& OSection : Other.Sections)
		{
			if (Filter.IsEmpty() || Filter.Contains(OSection.Key))
			{
				if (!Filter.IsEmpty())
				{
					UE_LOG(LogProjectLink, Display, TEXT("\t\t- Section: %s"), *OSection.Key);
				}

				Sections.FindOrAdd(OSection.Key).Append(OSection.Value, Filter.FindRef(OSection.Key));
			}
		}
	}
};