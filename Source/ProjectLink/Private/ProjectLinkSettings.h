// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ProjectLinkTypes.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectLinkSettings.generated.h"

UCLASS(DefaultConfig, Config = ProjectLink, DisplayName = "Project Link")
class UProjectLinkSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UProjectLinkSettings(): bEnableProjectLink(false), bCopyUProjectPlugins(true)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ProjectLink");

		ProjectsDir.Path = TEXT("C:/UnrealEngine/UnrealProjects");
	}

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (ConfigRestartRequired = true))
		bool bEnableProjectLink;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		FDirectoryPath ProjectsDir;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		bool bCopyUProjectPlugins;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		TSet<FPLRelativePath> SymlinkDirs;

	UPROPERTY(Config, EditAnywhere, Category = ProjectLink, meta = (EditCondition = bEnableProjectLink))
		TSet<FPLConfigEntry> BaseConfigs;
};