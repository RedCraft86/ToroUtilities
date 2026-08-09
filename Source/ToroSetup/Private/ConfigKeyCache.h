// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Types/RelativePath.h"

class FTSetupConfigKeyCache final
{
	static inline TMap<FString, TMap<FString, TSet<FString>>> FileData = {};

public:

	static void ForEachSection(const FString& Path, const TFunction<void(const FString&)>& Callback)
	{
		if (Path.IsEmpty())
		{
			return;
		}

		if (!FileData.Contains(Path))
		{
			CacheFile(Path);
		}

		TArray<FString> Values;
		FileData.FindOrAdd(Path).GenerateKeyArray(Values);
		for (const FString& Value : Values)
		{
			Callback(Value);
		}
	}

	static void ForEachEntry(const FString& Path, const TFunction<void(const FString&)>& Callback)
	{
		if (Path.IsEmpty())
		{
			return;
		}

		FString File, Section;
		Path.Split(TEXT(":"), &File, &Section, ESearchCase::IgnoreCase);

		if (!FileData.Contains(File))
		{
			CacheFile(File);
		}

		TArray<FString> Values = FileData.FindOrAdd(File).FindOrAdd(Section).Array();
		for (const FString& Value : Values)
		{
			Callback(Value);
		}
	}

	static void CacheFile(const FString& File)
	{
		if (File.IsEmpty())
		{
			return;
		}

		static const TSet<FString> ArrayPrefixes { TEXT("+"), TEXT("-"), TEXT("."), TEXT("!") };

		TMap<FString, TSet<FString>>& FileKeys = FileData.FindOrAdd(File);

		TArray<FString> FileContent;
		if (!FFileHelper::LoadFileToStringArray(FileContent,
			*FTSetupRelativePath(File).MakeRelativeTo()))
		{
			FileKeys.Empty();
			return;
		}

		FileKeys.Empty(FileContent.Num());

		FString ActiveSection;
		for (const FString& FileLine : FileContent)
		{
			const FString Line = FileLine.TrimStartAndEnd();
			if (Line.StartsWith(TEXT("[")) && Line.EndsWith(TEXT("]")))
			{
				ActiveSection = Line.Mid(1, Line.Len() - 2);
				FileKeys.FindOrAdd(ActiveSection);
			}
			else if (!ActiveSection.IsEmpty())
			{
				FString Key;
				if (!Line.Split(TEXT("="), &Key, nullptr, ESearchCase::IgnoreCase))
				{
					continue;
				}

				Key.TrimStartAndEndInline();
				if (Key.IsEmpty())
				{
					continue;
				}

				if (ArrayPrefixes.Contains(Key.Left(1)))
				{
					Key.MidInline(1, Key.Len() - 1);
				}

				FileKeys.FindOrAdd(ActiveSection).Add(Key);
			}
		}
	}
};
