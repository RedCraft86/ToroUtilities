// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "ProjectLink.h"
#include "ProjectLinkConfig.h"
#include "ProjectLinkSettings.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/PlatformFileManager.h"
#include "DesktopPlatformModule.h"
#include "RelativePathDetails.h"
#include "JsonObjectWrapper.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Misc/App.h"

#define LOCTEXT_NAMESPACE "ProjectLink"

DEFINE_LOG_CATEGORY(LogProjectLink)

FString FProjectLinkModule::PickFileOrDir(const FString& StartDir, const FString& Title, const FString& FileTypes)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		UE_LOG(LogProjectLink, Error, TEXT("DesktopPlatform module not available."));
		return FString();
	}
    
	const void* WinHandle = nullptr;
	if (FSlateApplication::IsInitialized())
	{
		const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindBestParentWindowForDialogs(nullptr);
		if (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
		{
			WinHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> OutPaths;
	if (FileTypes.IsEmpty())
	{
		FString Folder;
		if (DesktopPlatform->OpenDirectoryDialog(WinHandle, Title, StartDir, Folder) && !Folder.IsEmpty())
		{
			OutPaths = { Folder };
		}
	}
	else
	{
		const FString Filter = FileTypes == TEXT("*") ? TEXT("All Files (*.*)|*.*") : FileTypes;
		if (DesktopPlatform->OpenFileDialog(WinHandle, Title, StartDir, FString(), 
			Filter, EFileDialogFlags::None, OutPaths) && !OutPaths.IsEmpty())
		{
			OutPaths[0] = FPaths::ConvertRelativePathToFull(OutPaths[0]);
		}
	}

	return OutPaths.IsEmpty() ? FString() : OutPaths[0];
}

void FProjectLinkModule::StartupModule()
{
	Settings = GetDefault<UProjectLinkSettings>();
	if (Settings && Settings->bEnableProjectLink)
	{
		FToolbarButtonRegistry::AddButton<FProjectLinkButton>();
	}

	FDetailsCustomizationRegistry::Register<FPLRelativePath, FRelativePathDetails>();
}

void FProjectLinkButton::Execute()
{
	if (FProjectLinkModule::Settings)
	{
		const FString OutFile = FProjectLinkModule::PickFileOrDir(FProjectLinkModule::Settings->ProjectsDir.Path, 
			TEXT("Select Unreal Project File"), TEXT("Unreal Project Files (*.uproject)|*.uproject"));

		if (!FPaths::FileExists(OutFile))
		{
			UE_LOG(LogProjectLink, Display, TEXT("No .uproject file selected or dialog canceled."));
			return;
		}

		const FString SharedDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
		FString SharedProject = SharedDir / FString::Printf(TEXT("%s.uproject"), FApp::GetProjectName());
		FPaths::NormalizeFilename(SharedProject);

		FString TargetProject = FPaths::ConvertRelativePathToFull(OutFile);
		FPaths::NormalizeFilename(TargetProject);
		const FString TargetDir = FPaths::GetPath(TargetProject);

		if (TargetDir.Equals(SharedDir))
		{
			UE_LOG(LogProjectLink, Warning, TEXT("Target project cannot be the SharedProject"));
			return;
		}

		const FString TargetName = FPaths::GetCleanFilename(TargetProject);
		UE_LOG(LogProjectLink, Display, TEXT("======== Starting Link/Copy to: %s ========\n"), *TargetName);

		ProcessSymlinks(SharedDir, TargetDir);
		ProcessConfigs(SharedDir, TargetDir);
		if (FProjectLinkModule::Settings->bCopyUProjectPlugins)
		{
			ProcessPlugins(SharedProject, TargetProject);
		}

		UE_LOG(LogProjectLink, Display, TEXT("======== Finished Link/Copy to: %s ========\n"), *TargetName);
	}
	else
	{
		UE_LOG(LogProjectLink, Display, TEXT("Failed Link/Copy because ProjectLinkSettings... is null?"));
	}
}

void FProjectLinkButton::ProcessSymlinks(const FString& SrcDir, const FString& DestDir)
{
	UE_LOG(LogProjectLink, Display, TEXT("Begin Processing Symlinks..."));

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	for (const FPLRelativePath& RelativeDir : FProjectLinkModule::Settings->SymlinkDirs)
	{
		const FString Source = RelativeDir.MakeRelativeTo(SrcDir);
		const FString Target = RelativeDir.MakeRelativeTo(DestDir);
		if (PlatformFile.DirectoryExists(*Target))
		{
			if (PlatformFile.IsSymlink(*Target) == ESymlinkResult::Symlink)
			{
				UE_LOG(LogProjectLink, Warning, TEXT("\tFolder is Already a Symlink at Target: %s"), *RelativeDir.Path);
				continue;
			}
	
			bool bIsEmpty = true;
			PlatformFile.IterateDirectory(*Target, [&bIsEmpty](const TCHAR*, bool)
			{
				bIsEmpty = false;
				return false;
			});
	
			if (!bIsEmpty)
			{
				UE_LOG(LogProjectLink, Warning, TEXT("\tFolder exists with Contents at Target: %s"), *RelativeDir.Path);
				continue;
			}
	
			PlatformFile.DeleteDirectory(*Target);
		}
		else
		{
			PlatformFile.CreateDirectoryTree(*FPaths::GetPath(Target));
		}
		
		if (FPlatformProcess::CreateProc(TEXT("C:\\Windows\\System32\\cmd.exe"),
			*FString::Printf(TEXT("/c mklink /J \"%s\" \"%s\""), *Target, *Source),
			true, true, false, nullptr, 0, nullptr, nullptr).IsValid())
		{
			UE_LOG(LogProjectLink, Display, TEXT("\tSuccessfully Linked: %s"), *RelativeDir.Path);
		}
		else
		{
			UE_LOG(LogProjectLink, Warning, TEXT("\tFailed to Link: %s"), *RelativeDir.Path);
		}
	}

	UE_LOG(LogProjectLink, Display, TEXT("Finish Processing Symlinks..."));
}

void FProjectLinkButton::ProcessConfigs(const FString& SrcDir, const FString& DestDir)
{
	UE_LOG(LogProjectLink, Display, TEXT("Begin Processing Configs..."));

	for (const FPLConfigEntry& ConfigEntry : FProjectLinkModule::Settings->BaseConfigs)
	{
		if (!ConfigEntry.IsValid())
		{
			continue;
		}

		UE_LOG(LogProjectLink, Display, TEXT("\tCopying: %s"), *ConfigEntry.File.Path);

		const FString Source = ConfigEntry.File.MakeRelativeTo(SrcDir);
		const FString Target = ConfigEntry.File.MakeRelativeTo(DestDir);

		FPLConfigFile SrcFile;
		SrcFile.LoadFile(Source);

		FPLConfigFile DstFile;
		DstFile.LoadFile(Target);
		if (ConfigEntry.Sections.IsEmpty())
		{
			DstFile = SrcFile;
			UE_LOG(LogProjectLink, Display, TEXT("\t\t- Entire File"));
		}
		else
		{
			DstFile.Append(SrcFile, ConfigEntry.GenerateFilterMap());
		}

		DstFile.SaveFile(Target);
	}

	UE_LOG(LogProjectLink, Display, TEXT("Finish Processing Configs..."));
}

void FProjectLinkButton::ProcessPlugins(const FString& SrcFile, const FString& DestFile)
{
	UE_LOG(LogProjectLink, Display, TEXT("Begin Processing UProject Plugins..."));

	static const FString PluginName(TEXT("Name"));
	static const FString PluginEnabled(TEXT("Enabled"));
	static const FString PluginsField(TEXT("Plugins"));
	if (SrcFile.EndsWith(TEXT(".uproject")) && DestFile.EndsWith(TEXT(".uproject")))
	{
		FString SrcData;
		if (FFileHelper::LoadFileToString(SrcData, *SrcFile))
		{
			FJsonObjectWrapper SrcJson;
			SrcJson.JsonObjectFromString(SrcData);
			if (SrcJson.JsonObject->HasField(PluginsField))
			{
				const TArray<TSharedPtr<FJsonValue>>& PluginEntries = SrcJson.JsonObject->GetArrayField(PluginsField);
				for (const TSharedPtr<FJsonValue>& PluginEntry : PluginEntries)
				{
					const TSharedPtr<FJsonObject>* AsObject;
					if (PluginEntry->TryGetObject(AsObject) && AsObject)
					{
						FString PluginNameStr;
						bool bEnabled = false;
						if (AsObject->Get()->TryGetStringField(PluginName, PluginNameStr) 
						 && AsObject->Get()->TryGetBoolField(PluginEnabled, bEnabled))
						{
							UE_LOG(LogProjectLink, Display, TEXT("\t\t- %s: %s"), 
								*PluginNameStr, *LexToString(bEnabled));
						}
					}
				}

				FString DestData;
				if (FFileHelper::LoadFileToString(DestData, *DestFile))
				{
					FJsonObjectWrapper DestJson;
					DestJson.JsonObjectFromString(DestData);

					DestJson.JsonObject->SetArrayField(PluginsField, PluginEntries);

					DestData.Empty();
					const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&DestData, 0);
					FJsonSerializer::Serialize(DestJson.JsonObject.ToSharedRef(), JsonWriter, true);
					FFileHelper::SaveStringToFile(DestData, *DestFile);
				}
			}
		}
	}

	UE_LOG(LogProjectLink, Display, TEXT("Finish Processing UProject Plugins..."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProjectLinkModule, ProjectLink)