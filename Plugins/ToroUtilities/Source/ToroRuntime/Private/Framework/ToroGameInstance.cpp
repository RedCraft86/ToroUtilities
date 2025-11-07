// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "GeneralProjectSettings.h"
#include "Helpers/WindowsHelpers.h"

void UToroGameInstance::SetUnlitViewmode(const bool bUnlit)
{
	if (bUnlitMode != bUnlit)
	{
		bUnlitMode = bUnlit;
		const UWorld* World = GetWorld();
		if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
		{
			Viewport->ViewModeIndex = bUnlit ? VMI_Unlit : VMI_Lit;
		}
	}
}

void UToroGameInstance::SetPlayerInvincible(const bool bInvincible)
{
	if (bInvinciblePlayer != bInvincible)
	{
		bInvinciblePlayer = bInvincible;
		OnPlayerInvincible.Broadcast(bInvinciblePlayer);
	}
}

void UToroGameInstance::OnSettingUpdate(const ESettingApplyType Type)
{
	if (Type == ESettingApplyType::Developer)
	{
		const UToroUserSettings* UserSettings = UToroUserSettings::Get();
		if (!UserSettings || !UserSettings->GetDeveloperMode())
		{
			SetUnlitViewmode(false);
			SetPlayerInvincible(false);
		}
	}
}

void UToroGameInstance::OnFirstLaunch()
{
	const UWorld* World = GetWorld();
	if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
	{
		Viewport->ViewModeIndex = VMI_Lit;
	}
}

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	UToroUserSettings* UserSettings = UToroUserSettings::Get();
	if (UserSettings && UserSettings->InitializeSettings(this))
	{
		OnFirstLaunch();
		UserSettings->OnSettingsUpdated.AddUObject(this, &UToroGameInstance::OnSettingUpdate);
	}
}

void UToroGameInstance::Init()
{
	Super::Init();
#if UE_BUILD_SHIPPING
	// Only allow one game instance to run
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		if (!IFileManager::Get().CreateFileWriter(*(FPaths::ProjectSavedDir() / ProjectSettings->ProjectID.ToString()), 0))
		{
			WindowsHelpers::OpenDialogue(TEXT("Failed to Launch Game"),
				TEXT("An instance of this game is already running!"),
				EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);

			FPlatformApplicationMisc::RequestMinimize();
			FPlatformMisc::RequestExit(false);
		}
	}
#endif
}
