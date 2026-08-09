// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Types/ConfigInfo.h"
#include "Types/RelativePath.h"
#include "Engine/DeveloperSettings.h"
#include "ToroSetupSettings.generated.h"

UCLASS(DefaultConfig, Config = ToroSetup, DisplayName = "ToroSetup")
class UToroSetupSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSetupSettings()
		: bEnableFeature(false), bCopyProjectPlugins(true)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroSetup");

		ProjectsDir.Path = TEXT("D:/UnrealEngine/UnrealProjects");
	}

	UPROPERTY(Config, EditAnywhere, Category = ToroSetup, meta = (ConfigRestartRequired = true))
		bool bEnableFeature;

	UPROPERTY(Config, EditAnywhere, Category = ToroSetup, meta = (EditCondition = bEnableFeature))
		FDirectoryPath ProjectsDir;

	UPROPERTY(Config, EditAnywhere, Category = ToroSetup, meta = (EditCondition = bEnableFeature))
		bool bCopyProjectPlugins;

	UPROPERTY(Config, EditAnywhere, Category = ToroSetup, meta = (EditCondition = bEnableFeature))
		TSet<FTSetupRelativePath> SymlinkDirs;

	UPROPERTY(Config, EditAnywhere, Category = ToroSetup, meta = (EditCondition = bEnableFeature))
		TSet<FTSetupConfigInfo> IniConfigs;

	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		for (FTSetupConfigInfo& Info : IniConfigs)
		{
			Info.ResolveOptions();
		}
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		for (FTSetupConfigInfo& Info : IniConfigs)
		{
			Info.ResolveOptions();
		}
	}
};
