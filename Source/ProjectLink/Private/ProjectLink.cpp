// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ProjectLink.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "HAL/PlatformFileManager.h"
#include "Framework/Application/SlateApplication.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"
#include "ProjectLinkSettings.h"
#include "ProjectConfigData.h"

#define LOCTEXT_NAMESPACE "ProjectLink"

DEFINE_LOG_CATEGORY(LogProjectLink)

void FProjectLinkModule::StartupModule()
{
	Settings = GetDefault<UProjectLinkSettings>();
	if (Settings && Settings->bEnableProjectLink)
	{
		FToolbarButtonRegistry::AddButton<FProjectLinkButton>();
	}
}

void FProjectLinkButton::Execute()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		UE_LOG(LogProjectLink, Error, TEXT("DesktopPlatform module not available."));
		return;
	}
    
	const void* ParentWindowHandle = nullptr;
	if (FSlateApplication::IsInitialized())
	{
		const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindBestParentWindowForDialogs(nullptr);
		if (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
		{
			ParentWindowHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}
    
	TArray<FString> OutFiles;
	const bool bFileSelected = DesktopPlatform->OpenFileDialog(ParentWindowHandle, 
		TEXT("Select Unreal Project File"), FProjectLinkModule::Settings->ProjectsFolder.Path, FString(), 
		TEXT("Unreal Project Files (*.uproject)|*.uproject"), EFileDialogFlags::None, OutFiles
	);
	
	if (!bFileSelected || OutFiles.IsEmpty() || !FPaths::FileExists(OutFiles[0]))
	{
		UE_LOG(LogProjectLink, Display, TEXT("No .uproject file selected or dialog canceled."));
		return;
	}

	const FString ProjectPath = FPaths::ConvertRelativePathToFull(OutFiles[0]);
	if (ProjectPath.EndsWith(TEXT("SharedProject.uproject")))
	{
		UE_LOG(LogProjectLink, Warning, TEXT("Selected project cannot be the SharedProject"));
		return;
	}

	const FString ProjectName = FPaths::GetCleanFilename(ProjectPath);
	UE_LOG(LogProjectLink, Display, TEXT("======== Starting Link/Copy to: %s ========\n"), *ProjectName);

	if (FProjectLinkModule::Settings)
	{
		const FString SrcPath = FPaths::ProjectDir();
		const FString DestPath = FPaths::GetPath(ProjectPath);
		ProcessSymlinks(SrcPath, DestPath);
		ProcessConfigs(SrcPath, DestPath);

		UE_LOG(LogProjectLink, Display, TEXT("======== Finished Link/Copy to: %s ========\n"), *ProjectName);
	}
	else
	{
		UE_LOG(LogProjectLink, Display, TEXT("======== Failed Link/Copy to: %s ========\n"), *ProjectName);
	}
}

void FProjectLinkButton::ProcessSymlinks(const FString& SrcPath, const FString& DestPath)
{
	UE_LOG(LogProjectLink, Display, TEXT("Begin Processing Symlinks..."));

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	for (const FPLSymlinkPath& Path : FProjectLinkModule::Settings->SymlinkDirs)
	{
		const FString SrcRelative = Path.GetRelativePath().TrimStartAndEnd();
		const FString DestRelative = Path.GetTargetRelativePath().TrimStartAndEnd();
		if (SrcRelative.IsEmpty() || DestRelative.IsEmpty())
		{
			continue;
		}

		const FString Source = SrcPath / SrcRelative;
		const FString Link = DestPath / DestRelative;

		if (PlatformFile.DirectoryExists(*Link))
		{
			if (PlatformFile.IsSymlink(*Link) == ESymlinkResult::Symlink)
			{
				UE_LOG(LogProjectLink, Warning, TEXT("\tFolder is Already a Symlink: %s <-> %s"), *SrcRelative, *DestRelative);
				continue;
			}

			bool bIsEmpty = true;
			PlatformFile.IterateDirectory(*Link, [&bIsEmpty](const TCHAR*, bool)
			{
				bIsEmpty = false;
				return false;
			});

			if (!bIsEmpty)
			{
				UE_LOG(LogProjectLink, Warning, TEXT("\tFolder exists with Contents: %s <-> %s"), *SrcRelative, *DestRelative);
				continue;
			}

			PlatformFile.DeleteDirectory(*Link);
		}
		else
		{
			PlatformFile.CreateDirectoryTree(*FPaths::GetPath(Link));
		}

		if (FPlatformProcess::CreateProc(TEXT("C:\\Windows\\System32\\cmd.exe"),
			*FString::Printf(TEXT("/c mklink /J \"%s\" \"%s\""), *Link, *Source),
			true, true, false, nullptr, 0, nullptr, nullptr).IsValid())
		{
			UE_LOG(LogProjectLink, Display, TEXT("\tSuccessfully Linked: %s <-> %s"), *SrcRelative, *DestRelative);
		}
		else
		{
			UE_LOG(LogProjectLink, Warning, TEXT("\tFailed to Link: %s <-> %s"), *SrcRelative, *DestRelative);
		}
	}

	UE_LOG(LogProjectLink, Display, TEXT("Finish Processing Symlinks..."));
}

void FProjectLinkButton::ProcessConfigs(const FString& SrcPath, const FString& DestPath)
{
	UE_LOG(LogProjectLink, Display, TEXT("Begin Processing Configs..."));

	for (const auto& [Config, Sections] : FProjectLinkModule::Settings->CopyConfigs)
	{
		if (Config.FilePath.IsEmpty())
		{
			continue;
		}

		UE_LOG(LogProjectLink, Display, TEXT("\tCopying: %s"), *Config.FilePath);

		const FString Source = SrcPath / Config.FilePath;
		const FString Dest = DestPath / Config.FilePath;
		
		FPLConfigFile SrcCfg;
		SrcCfg.LoadFromFile(Source);

		FPLConfigFile DestCfg;
		if (Sections.IsEmpty())
		{
			DestCfg = SrcCfg;
			UE_LOG(LogProjectLink, Display, TEXT("\t\t- Entire File"));
		}
		else
		{
			DestCfg.LoadFromFile(Dest);
			for (const auto& [Section, Keys] : Sections)
			{
				if (FPLConfigEntries* SrcData = SrcCfg.Find(Section))
				{
					FPLConfigEntries& DestData = DestCfg.FindOrAdd(Section);
					if (Keys.IsEmpty())
					{
						DestData.Append(*SrcData);
						UE_LOG(LogProjectLink, Display, TEXT("\t\t- Entire Section: %s"), *Section);
					}
					else
					{
						UE_LOG(LogProjectLink, Display, TEXT("\t\t- Under Section: %s"), *Section);
						for (const FString& Key : Keys)
						{
							if (const FString* SrcValue = SrcData->Find(Key))
							{
								DestData.SetOrAddValue(Key, *SrcValue);
								UE_LOG(LogProjectLink, Display, TEXT("\t\t\t- Property: %s"), *Key);
							}
						}
					}
				}
			}
		}
		
		DestCfg.SaveToFile(Dest);

		UE_LOG(LogProjectLink, Display, TEXT("\tConfig Successfully Copied"));
	}

	UE_LOG(LogProjectLink, Display, TEXT("Finish Processing Configs..."));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FProjectLinkModule, ProjectLink)