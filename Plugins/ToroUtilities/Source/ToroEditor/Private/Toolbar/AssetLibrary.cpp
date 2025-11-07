// Copyright (C) RedCraft86. All Rights Reserved.

#include "Toolbar/AssetLibrary.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "ToroEditorSettings.h"

void FAssetLibrary::ExecuteAction()
{
	bool bSuccess = false;
	const TMap<FString, FString> Links = UToroEditorSettings::Get()->GetAssetLibraryPaths();
	for (const TPair<FString, FString>& Link : Links)
	{
		if (!FPaths::DirectoryExists(Link.Key)) continue;
		if (FPaths::DirectoryExists(Link.Value))
		{
			IFileManager& FileManager = IFileManager::Get();

			TArray<FString> FoundItems;
			FileManager.FindFiles(FoundItems, *(Link.Value / TEXT("*")), true, true);
			if (FoundItems.IsEmpty())
			{
				FileManager.DeleteDirectory(*Link.Value);
			}
			else
			{
				ShowErrorMsg(TEXT("Folder Already Exist!"), Link.Value);
				continue;
			}
		}

		int32 RetCode;
		FString Message;
		const FString Command = FString::Printf(TEXT("cmd /c mklink /J \"%s\" \"%s\""), *Link.Value, *Link.Key);
		FPlatformProcess::ExecProcess(TEXT("cmd.exe"), *Command, &RetCode, &Message, nullptr);
		if (RetCode != 0)
		{
			ShowErrorMsg(Message, Link.Value);
			continue;
		}

		bSuccess = true;
	}

	if (bSuccess)
	{
		FUnrealEdMisc::Get().RestartEditor(false);
	}
}

void FAssetLibrary::ShowErrorMsg(const FString& Reason, const FString& Path)
{
	FString SubPath = TEXT("");
	Path.Split(FApp::GetProjectName(), nullptr, &SubPath);
	if (!SubPath.IsEmpty())
	{
		SubPath = TEXT("...") / SubPath;
	}
	else SubPath = Path;
		
	FNotificationInfo Info(FText::Format(
		INVTEXT("Failed to create junction link!\n{0}\n\"{1}\""),
		FText::FromString(Reason), FText::FromString(SubPath))
	);
	Info.ExpireDuration = 2.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}
