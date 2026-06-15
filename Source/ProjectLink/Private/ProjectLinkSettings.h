// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Misc/Paths.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectLinkSettings.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FPLSymlinkPath final
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = Symlink)
		FDirectoryPath Directory;

	UPROPERTY(EditAnywhere, Category = Symlink)
		FString Alias;

public:

	FORCEINLINE friend uint32 GetTypeHash(const FPLSymlinkPath& InPath)
	{
		return GetTypeHash(InPath.Directory.Path);
	}

	FString GetRelativePath() const
	{
		if (!Directory.Path.IsEmpty())
		{
			return Directory.Path.Mid(FPaths::ProjectDir().Len());
		}

		return FString();
	}

	FString GetTargetRelativePath() const
	{
		return Alias.IsEmpty() ? GetRelativePath() : Alias;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPLConfigSectionEntry final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = SectionEntry)
		FString Section;

	UPROPERTY(EditAnywhere, Category = SectionEntry)
		TSet<FString> Keys;

	FORCEINLINE friend uint32 GetTypeHash(const FPLConfigSectionEntry& InEntry)
	{
		return GetTypeHash(InEntry.Section);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPLConfigCopyPath final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = ConfigCopy, meta = (RelativeToGameDir))
		FFilePath Config;

	UPROPERTY(EditAnywhere, Category = ConfigCopy)
		TSet<FPLConfigSectionEntry> Sections;

	FORCEINLINE friend uint32 GetTypeHash(const FPLConfigCopyPath& InPath)
	{
		return GetTypeHash(InPath.Config.FilePath);
	}
};

UCLASS(DefaultConfig, Config = ProjectLink, DisplayName = "Project Link")
class UProjectLinkSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UProjectLinkSettings(): bEnableProjectLink(false)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ProjectLink");

		ProjectsFolder.Path = TEXT("C:/UnrealEngine/UnrealProjects");
	}

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (ConfigRestartRequired = true))
		bool bEnableProjectLink;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		FDirectoryPath ProjectsFolder;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		TSet<FPLSymlinkPath> SymlinkDirs;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		TSet<FPLConfigCopyPath> CopyConfigs;
};