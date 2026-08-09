// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroSetupHelpers.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FToroSetupButton final : public FToroToolbarButton
{
public:

	FToroSetupButton()
		: FToroToolbarButton(TEXT("ToroSetup"), INVTEXT("Project Setup"),
			INVTEXT("Creates symlinks, copies specified base configs, and uproject plugins to a target project"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.ModesToolBar");
		SectionName = TEXT("File");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FCoreStyle::Get().GetStyleSetName(), TEXT("Icons.Toolbar.Export"));
	}

	virtual void Execute() override
	{
		const UToroSetupSettings* Settings = GetDefault<UToroSetupSettings>();
		if (!Settings || !Settings->bEnableFeature)
		{
			UE_LOG(LogToroSetup, Error, TEXT("Cannot start ToroSetup. Settings was null or feature was disabled."));
			return;
		}

		const FString OutFile = FToroSetupModule::PickFileOrDir(Settings->ProjectsDir.Path,
			TEXT("Select Unreal Project File"), TEXT("Unreal Project Files (*.uproject)|*.uproject"));

		if (!FPaths::FileExists(OutFile))
		{
			UE_LOG(LogToroSetup, Display, TEXT("No .uproject file selected or dialog canceled."));
			return;
		}

		const FString TargetDir = FPaths::GetPath(OutFile) / FString();
		const FString ProjectName = FPaths::GetBaseFilename(OutFile);
		if (ProjectName == FApp::GetProjectName())
		{
			// If names happen to match, make sure we're not looking at the same project.
			if (TargetDir == FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()))
			{
				UE_LOG(LogToroSetup, Warning, TEXT("Target project cannot be this project."));
				return;
			}
		}

		UE_LOG(LogToroSetup, Display, TEXT("\n======== Starting Setup for: %s ========\n"), *ProjectName);

		if (Settings->bCopyProjectPlugins)
		{
			UE_LOG(LogToroSetup, Display, TEXT("======== Starting Plugins Copy To: %s ========"), *ProjectName);

			ToroSetupHelpers::HandlePlugins(TargetDir, ProjectName);

			UE_LOG(LogToroSetup, Display, TEXT("======== Finished Plugins Copy To: %s ========"), *ProjectName);
		}

		if (!Settings->SymlinkDirs.IsEmpty())
		{
			UE_LOG(LogToroSetup, Display, TEXT("======== Starting Symlink Creation To: %s ========"), *ProjectName);

			for (const FTSetupRelativePath& SymlinkPath : Settings->SymlinkDirs)
			{
				ToroSetupHelpers::HandleSymlink(TargetDir, SymlinkPath);
			}

			UE_LOG(LogToroSetup, Display, TEXT("======== Finished Symlink Creation To: %s ========"), *ProjectName);
		}

		if (!Settings->IniConfigs.IsEmpty())
		{
			UE_LOG(LogToroSetup, Display, TEXT("======== Starting IniConfig Copy To: %s ========"), *ProjectName);

			bool bIsFirst = true;
			for (const FTSetupConfigInfo& ConfigInfo : Settings->IniConfigs)
			{
				if (!bIsFirst)
				{
					UE_LOG(LogToroSetup, Display, TEXT("\t-"));
				}

				bIsFirst = false;
				ToroSetupHelpers::HandleIniConfig(TargetDir, ConfigInfo);
			}

			UE_LOG(LogToroSetup, Display, TEXT("======== Finished IniConfig Copy To: %s ========"), *ProjectName);
		}

		UE_LOG(LogToroSetup, Display, TEXT("\n======== Finished Setup for: %s ========\n"), *ProjectName);
	}
};