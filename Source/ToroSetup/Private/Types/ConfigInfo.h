// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "RelativePath.h"
#include "ConfigInfo.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FTSetupConfigEntry final
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Config)
		FString Key;

	UPROPERTY(Transient, VisibleDefaultsOnly)
		FString OptionSource;

	FTSetupConfigEntry()
		: Key(FString()), OptionSource(FString())
	{}

	FORCEINLINE bool operator==(const FTSetupConfigEntry& Other) const
	{
		return Key == Other.Key;
	}

	FORCEINLINE bool operator!=(const FTSetupConfigEntry& Other) const
	{
		return Key != Other.Key;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FTSetupConfigEntry& InEntry)
	{
		return GetTypeHash(InEntry.Key);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FTSetupConfigSection final
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Config)
		FString Section;

	UPROPERTY(Config, EditAnywhere, Category = Config)
		TSet<FTSetupConfigEntry> Entries;

	UPROPERTY(Transient, VisibleDefaultsOnly)
		FString OptionSource;

	FTSetupConfigSection()
		: Section(FString()), Entries({}), OptionSource(FString())
	{}

	void ResolveOptions(const FString& File)
	{
		OptionSource = File;
		for (FTSetupConfigEntry& Entry : Entries)
		{
			Entry.OptionSource = FString::Printf(TEXT("%s:%s"), *File, *Section);
		}
	}

	FORCEINLINE bool operator==(const FTSetupConfigSection& Other) const
	{
		return Section == Other.Section;
	}

	FORCEINLINE bool operator!=(const FTSetupConfigSection& Other) const
	{
		return Section != Other.Section;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FTSetupConfigSection& InSection)
	{
		return GetTypeHash(InSection.Section);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FTSetupConfigInfo final
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Config, meta = (FileTypes = "Config Files (*.ini)|*.ini"))
		FTSetupRelativePath File;

	UPROPERTY(Config, EditAnywhere, Category = Config)
		TSet<FTSetupConfigSection> Sections;

	FTSetupConfigInfo()
		: File(FString()), Sections({})
	{}

	void ResolveOptions()
	{
		for (FTSetupConfigSection& Section : Sections)
		{
			Section.ResolveOptions(File.Path);
		}
	}

	TMap<FString, TSet<FString>> FlattenConfig() const
	{
		TMap<FString, TSet<FString>> Result;
		for (const FTSetupConfigSection& Section : Sections)
		{
			TSet<FString>& Entries = Result.FindOrAdd(Section.Section);
			for (const FTSetupConfigEntry& Entry : Section.Entries)
			{
				Entries.Add(Entry.Key);
			}
		}

		return Result;
	}

	FORCEINLINE bool operator==(const FTSetupConfigInfo& Other) const
	{
		return File == Other.File;
	}

	FORCEINLINE bool operator!=(const FTSetupConfigInfo& Other) const
	{
		return File != Other.File;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FTSetupConfigInfo& InInfo)
	{
		return GetTypeHash(InInfo.File);
	}
};