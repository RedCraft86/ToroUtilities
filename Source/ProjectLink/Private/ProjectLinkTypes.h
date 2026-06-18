// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Misc/App.h"
#include "Misc/Paths.h"
#include "ProjectLinkTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FPLRelativePath
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Path)
		FString Path;

	FString MakeRelativeTo(const FString& BasePath) const
	{
		if (Path.IsEmpty())
		{
			return FString();
		}

		const FString CleanedBase = BasePath.TrimStartAndEnd();
		FString Result = CleanedBase.IsEmpty() ? FString() : (CleanedBase / Path);
		Result.TrimStartAndEndInline();

		if (!Result.IsEmpty())
		{
			FPaths::NormalizeDirectoryName(Result);
		}

		return Result;
	}

	FORCEINLINE bool operator==(const FPLRelativePath& Other) const
	{
		return Path == Other.Path;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FPLRelativePath& InPath)
	{
		return GetTypeHash(InPath.Path);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPLConfigSectionKeys
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Config)
		TArray<FString> Keys;

	FORCEINLINE const TArray<FString>& operator*() const
	{
		return Keys;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPLConfigEntry
{
    GENERATED_BODY()

    UPROPERTY(Config, EditAnywhere, Category = Config, meta = (FileTypes = "Config Files (*.ini)|*.ini"))
		FPLRelativePath File;

    UPROPERTY(Config, EditAnywhere, Category = Config)
		TMap<FString, FPLConfigSectionKeys> Sections;

	TMap<FString, TSet<FString>> GenerateFilterMap() const
	{
		TMap<FString, TSet<FString>> Result;
		for (const TPair<FString, FPLConfigSectionKeys>& Section : Sections)
		{
			Result.Add(Section.Key).Append(*Section.Value);
		}
		return Result;
	}

	FORCEINLINE bool IsValid() const
	{
		return !File.Path.IsEmpty() 
			&& File.Path.EndsWith(TEXT(".ini"));
	}

	FORCEINLINE bool operator==(const FPLConfigEntry& Other) const
	{
		return File == Other.File;
	}

    FORCEINLINE friend uint32 GetTypeHash(const FPLConfigEntry& InEntry)
    {
        return GetTypeHash(InEntry.File);
    }
};