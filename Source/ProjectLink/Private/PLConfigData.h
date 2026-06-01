// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"

class FPLConfigEntries final
{
public:

	TArray<TPair<FString, FString>> Entries;

	FString* Find(const FString& Section)
	{
		for (TPair<FString, FString>& Sec : Entries)
		{
			if (Sec.Key.Equals(Section))
			{
				return &Sec.Value;
			}
		}

		return nullptr;
	}

	void SetOrAddValue(const FString& Key, const FString& Value)
	{
		auto IsArray = [](const FString& KeyName)
		{
			return KeyName.StartsWith(TEXT("+")) || KeyName.StartsWith(TEXT("-"));
		};

		if (IsArray(Key))
		{
			int32 InsertIdx = 0;
			bool bFoundArray = false;
			for (const TPair<FString, FString>& Entry : Entries)
			{
				InsertIdx++;
				if (IsArray(Entry.Key) && Key.Mid(1).Equals(Entry.Key.Mid(1)))
				{
					bFoundArray = true;
				}
				else if (bFoundArray)
				{
					break;
				}
			}

			Entries.EmplaceAt(InsertIdx, Key, Value);
		}
		else
		{
			for (TPair<FString, FString>& Entry : Entries)
			{
				if (Entry.Key.Equals(Key))
				{
					Entry.Value = Value;
					return;
				}
			}

			Entries.Emplace(Key, Value);
		}
	}

	void Append(const FPLConfigEntries& InOtherEntries)
	{
		for (const TPair<FString, FString>& Entry : InOtherEntries.Entries)
		{
			SetOrAddValue(Entry.Key, Entry.Value);
		}
	}
};

class FPLConfigFile final
{
public:

	TArray<TPair<FString, FPLConfigEntries>> Sections;

	FPLConfigEntries* Find(const FString& Section)
	{
		for (TPair<FString, FPLConfigEntries>& Sec : Sections)
		{
			if (Sec.Key.Equals(Section))
			{
				return &Sec.Value;
			}
		}

		return nullptr;
	}

	FPLConfigEntries& FindOrAdd(const FString& Section)
	{
		for (TPair<FString, FPLConfigEntries>& Sec : Sections)
		{
			if (Sec.Key.Equals(Section))
			{
				return Sec.Value;
			}
		}

		return Sections.Emplace_GetRef(Section, FPLConfigEntries()).Value;
	}

	bool SaveToFile(const FString& FilePath)
	{
		FString Content;
		for (const TPair<FString, FPLConfigEntries>& Sec : Sections)
		{
			Content.Appendf(TEXT("[%s]\n"), *Sec.Key);
			for (const TPair<FString, FString>& Entries : Sec.Value.Entries)
			{
				Content.Appendf(TEXT("%s=%s\n"), *Entries.Key, *Entries.Value);
			}

			Content.Append(TEXT("\n"));
		}

		return FFileHelper::SaveStringToFile(Content, *FilePath);
	}

	bool LoadFromFile(const FString& FilePath)
	{
		FString Content;
		if (!FFileHelper::LoadFileToString(Content, *FilePath))
		{
			return false;
		}

		TArray<FString> Lines;
		Content.ParseIntoArrayLines(Lines, true);

		FString CurrentSection;
		for (FString& Line : Lines)
		{
			Line.TrimStartAndEndInline();
			if (Line.StartsWith(TEXT(";")) || Line.IsEmpty())
			{
				continue;
			}

			if (Line.StartsWith(TEXT("[")))
			{
				CurrentSection = Line.Mid(1, Line.Len() - 2);
				continue;
			}

			FString Key, Value;
			if (Line.Split(TEXT("="), &Key, &Value))
			{
				FindOrAdd(CurrentSection).Entries.Emplace(Key, Value);
			}
		}

		return true;
	}
};