// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroSetup.h"
#include "JsonObjectWrapper.h"
#include "Types/RelativePath.h"
#include "Types/ConfigInfo.h"
#include "Types/ConfigData.h"

namespace ToroSetupHelpers
{
	inline void HandlePlugins(const FString& TargetDir, const FString& ProjectName)
	{
		FString SourceJsonStr;
		FJsonObjectWrapper SourceJson;
		const FString SourceProject = FPaths::ProjectDir() / FApp::GetProjectName() + TEXT(".uproject");
		if (FFileHelper::LoadFileToString(SourceJsonStr, *SourceProject))
		{
			SourceJson.JsonObjectFromString(SourceJsonStr);
		}
		else
		{
			UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to read: %s"), *SourceProject);
			return;
		}

		FString TargetJsonStr;
		FJsonObjectWrapper TargetJson;
		const FString TargetProject = TargetDir / ProjectName + TEXT(".uproject");
		if (FFileHelper::LoadFileToString(TargetJsonStr, *TargetProject))
		{
			TargetJson.JsonObjectFromString(TargetJsonStr);
		}
		else
		{
			UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to read: %s"), *TargetProject);
			return;
		}

		static const FName FIELD_Plugins(TEXT("Plugins"));
		const TArray<TSharedPtr<FJsonValue>>* Plugins = nullptr;
		if (SourceJson.JsonObject->TryGetArrayField(FIELD_Plugins.ToString(), Plugins) && Plugins)
		{
			TargetJson.JsonObject->SetArrayField(FIELD_Plugins.ToString(), *Plugins);
		}

		const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&TargetJsonStr, 0);
		if (FJsonSerializer::Serialize(TargetJson.JsonObject.ToSharedRef(), JsonWriter, true))
		{
			if (FFileHelper::SaveStringToFile(TargetJsonStr, *TargetProject))
			{
				UE_LOG(LogToroSetup, Display, TEXT("\tCopied plugin configuration to project: %s"), *ProjectName);
			}
			else
			{
				UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to copy plugin configuration to project: %s"), *ProjectName);
			}
		}
	}

	inline void HandleSymlink(const FString& TargetDir, const FTSetupRelativePath& RelativeDir)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		const FString Target = RelativeDir.MakeRelativeTo(TargetDir);
		if (PlatformFile.DirectoryExists(*Target))
		{
			if (PlatformFile.IsSymlink(*Target) == ESymlinkResult::Symlink)
			{
				UE_LOG(LogToroSetup, Warning, TEXT("\tFolder is Already a Symlink at Target: %s"), *RelativeDir.Path);
				return;
			}

			bool bIsEmpty = true;
			PlatformFile.IterateDirectory(*Target, [&bIsEmpty](const TCHAR*, bool)
			{
				bIsEmpty = false;
				return false;
			});

			if (!bIsEmpty)
			{
				UE_LOG(LogToroSetup, Warning, TEXT("\tFolder exists with Contents at Target: %s"), *RelativeDir.Path);
				return;
			}

			PlatformFile.DeleteDirectory(*Target);
		}
		else
		{
			PlatformFile.CreateDirectoryTree(*FPaths::GetPath(Target));
		}

		const FString Source = RelativeDir.MakeRelativeTo();
		if (FPlatformProcess::CreateProc(TEXT("C:\\Windows\\System32\\cmd.exe"),
			*FString::Printf(TEXT("/c mklink /J \"%s\" \"%s\""), *Target, *Source),
			true, true, false, nullptr, 0, nullptr, nullptr).IsValid())
		{
			UE_LOG(LogToroSetup, Display, TEXT("\tSuccessfully Linked: %s"), *RelativeDir.Path);
		}
		else
		{
			UE_LOG(LogToroSetup, Warning, TEXT("\tFailed to Link: %s"), *RelativeDir.Path);
		}
	}

	inline void HandleIniConfig(const FString& TargetDir, const FTSetupConfigInfo& ConfigInfo)
	{
		FTSetupConfigFileData SourceData;
		SourceData.LoadConfig(ConfigInfo.File.MakeRelativeTo(), ConfigInfo.FlattenConfig());

		const FString Target = ConfigInfo.File.MakeRelativeTo(TargetDir);
		FTSetupConfigFileData TargetData;
		TargetData.LoadConfig(Target);

		TargetData.Append(SourceData);
		TargetData.SaveConfig(Target);
	}
}
