// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ToroSetup.h"
#include "ToroSetupSettings.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"

#include "ToroSetupButton.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "Details/RelativePathDetails.h"
#include "Details/ConfigSectionDetails.h"
#include "Details/ConfigEntryDetails.h"

DEFINE_LOG_CATEGORY(LogToroSetup);

#define LOCTEXT_NAMESPACE "ToroSetup"

void FToroSetupModule::StartupModule()
{
	const UToroSetupSettings* Settings = GetDefault<UToroSetupSettings>();
	if (Settings && Settings->bEnableFeature)
	{
		FToolbarButtonRegistry::AddButton<FToroSetupButton>();
	}

	FDetailsCustomizationRegistry::Register<FTSetupRelativePath, FTSetupRelativePathDetails>();
	FDetailsCustomizationRegistry::Register<FTSetupConfigSection, FTSetupConfigSectionDetails>();
	FDetailsCustomizationRegistry::Register<FTSetupConfigEntry, FTSetupConfigEntryDetails>();
}

void FToroSetupModule::ShutdownModule()
{
}

FString FToroSetupModule::PickFileOrDir(const FString& StartDir, const FString& Title, const FString& FileTypes)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		UE_LOG(LogToroSetup, Error, TEXT("DesktopPlatform module not available."));
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
		const FString Filter = (FileTypes == TEXT("*")) ? TEXT("All Files (*.*)|*.*") : FileTypes;
		if (DesktopPlatform->OpenFileDialog(WinHandle, Title, StartDir, FString(),
			Filter, EFileDialogFlags::None, OutPaths) && !OutPaths.IsEmpty())
		{
			OutPaths[0] = FPaths::ConvertRelativePathToFull(OutPaths[0]);
		}
	}

	return OutPaths.IsEmpty() ? FString() : FPaths::ConvertRelativePathToFull(OutPaths[0]);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroSetupModule, ToroSetup)