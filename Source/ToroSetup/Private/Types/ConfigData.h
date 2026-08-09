// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ArrayPair.h"

struct FTSetupConfigSectionData final
{
	TArrayPair<FString, FString> Values;
	TArrayPair<FString, TArrayPair<FString, FString>> Arrays;

	void Append(const FTSetupConfigSectionData& Other)
	{
		Values.Append(Other.Values);
		Arrays.Append(Other.Arrays);
	}

	void Parse(const FString& Line)
	{
		static const TSet<FString> ArrayKeys { TEXT("+"), TEXT("-"), TEXT("."), TEXT("!") };

		FString Key, Value;
		if (!Line.Split(TEXT("="), &Key, &Value, ESearchCase::IgnoreCase))
		{
			Key = Line;
		}

		const FString Prefix = Key.Left(1);
		if (ArrayKeys.Contains(Prefix))
		{
			Arrays.FindOrAdd(Key.Right(Key.Len() - 1)).Add(Value, Prefix);
		}
		else
		{
			Values.Add(Key, Value);
		}
	}
};

struct FTSetupConfigFileData final
{
	TArrayPair<FString, FTSetupConfigSectionData> Sections;

	void Append(const FTSetupConfigFileData& Other)
	{
		for (TPair<FString, FTSetupConfigSectionData> Pair : Other.Sections.GetValues())
		{
			Sections.FindOrAdd(Pair.Key).Append(Pair.Value);
		}
	}

	void LoadConfig(const FString& Filename, const TMap<FString, TSet<FString>>& Filter = {})
	{
		FString FilePath = Filename;
		FPaths::NormalizeFilename(FilePath);

		TArray<FString> FileLines;
		if (FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
		{
			UE_LOG(LogToroSetup, Display, TEXT("\tRead config file %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to read config file %s"), *FilePath);
			return;
		}

		FString ActiveSection;
		for (FString& Line : FileLines)
		{
			Line.TrimStartAndEndInline();
			if (Line.IsEmpty())
			{
				continue;
			}

			if (Line.StartsWith(TEXT("[")) && Line.EndsWith(TEXT("]")))
			{
				ActiveSection = Line.Mid(1, Line.Len() - 2);
				if (!Filter.IsEmpty() && !Filter.Contains(ActiveSection))
				{
					ActiveSection.Empty();
				}
			}
			else if (!ActiveSection.IsEmpty())
			{
				bool bAllowKey = Filter.IsEmpty();

				if (const TSet<FString>* Keys = Filter.Find(ActiveSection))
				{
					if (Keys->IsEmpty())
					{
						bAllowKey = true;
					}
					else
					{
						for (const FString& Key : *Keys)
						{
							if (Line.StartsWith(Key + TEXT("=")))
							{
								bAllowKey = true;
								break;
							}
						}
					}
				}

				if (bAllowKey)
				{
					Sections.FindOrAdd(ActiveSection).Parse(Line);
				}
			}
		}
	}

	void SaveConfig(const FString& Filename) const
	{
		FString FilePath = Filename;
		FPaths::NormalizeFilename(FilePath);

		TArray<FString> FileLines;
		for (const TPair<FString, FTSetupConfigSectionData>& Section : Sections.GetValues())
		{
			FileLines.Add(FString::Printf(TEXT("[%s]"), *Section.Key));

			for (const TPair<FString, FString>& Value : Section.Value.Values.GetValues())
			{
				FileLines.Add(FString::Printf(TEXT("%s=%s"), *Value.Key, *Value.Value));
			}

			for (const TPair<FString, TArrayPair<FString, FString>>& Array : Section.Value.Arrays.GetValues())
			{
				for (const TPair<FString, FString>& Entry : Array.Value.GetValues())
				{
					FileLines.Add(FString::Printf(TEXT("%s%s=%s"), *Entry.Value, *Array.Key, *Entry.Key));
				}
			}

			FileLines.Add(FString());
		}

		if (!FileLines.IsEmpty())
		{
			FileLines.RemoveAt(FileLines.Num() - 1);
		}

		if (FFileHelper::SaveStringArrayToFile(FileLines, *FilePath))
		{
			UE_LOG(LogToroSetup, Display, TEXT("\tCopied config file %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to load config file %s"), *FilePath);
		}
	}
};