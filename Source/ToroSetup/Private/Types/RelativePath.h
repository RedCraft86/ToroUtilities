// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "RelativePath.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FTSetupRelativePath final
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Path)
		FString Path;

	FTSetupRelativePath()
		: Path(FString())
	{}

	FTSetupRelativePath(const FString& InPath)
		: Path(InPath)
	{}

	FString MakeRelativeTo(const FString& BasePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir())) const
	{
		if (Path.IsEmpty())
		{
			return FString();
		}

		const FString CleanedBase = BasePath.TrimStartAndEnd();
		FString Result = CleanedBase.IsEmpty() ? FString() : (CleanedBase / Path);
		Result.ReplaceInline(TEXT("//"), TEXT("/"));
		Result.TrimStartAndEndInline();

		if (!Result.IsEmpty())
		{
			if (FPaths::GetExtension(Result).IsEmpty())
			{
				FPaths::NormalizeDirectoryName(Result);
			}
			else
			{
				FPaths::NormalizeFilename(Result);
			}
		}

		return Result;
	}

	FORCEINLINE bool operator==(const FTSetupRelativePath& Other) const
	{
		return Path == Other.Path;
	}

	FORCEINLINE bool operator!=(const FTSetupRelativePath& Other) const
	{
		return Path != Other.Path;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FTSetupRelativePath& InPath)
	{
		return GetTypeHash(InPath.Path);
	}
};