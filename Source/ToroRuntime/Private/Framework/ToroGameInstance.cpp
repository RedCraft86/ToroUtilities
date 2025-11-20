// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroGameInstance.h"
#include "ToroRuntime.h"
#if WITH_EDITOR
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "GeneralProjectSettings.h"
#endif

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

void UToroGameInstance::AddTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (UToroGameInstance* GI = UToroGameInstance::Get(ContextObject))
	{
		GI->TransientFlags.AddTag(InFlag);
	}
}

void UToroGameInstance::RemoveTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (UToroGameInstance* GI = UToroGameInstance::Get(ContextObject))
	{
		GI->TransientFlags.RemoveTag(InFlag);
	}
}

bool UToroGameInstance::HasTransientFlag(const UObject* ContextObject, const FGameplayTag InFlag)
{
	if (const UToroGameInstance* GI = UToroGameInstance::Get(ContextObject))
	{
		return GI->TransientFlags.HasTagExact(InFlag);
	}
	return false;
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
			FPlatformApplicationMisc::RequestMinimize();
			FPlatformMisc::RequestExit(false);
		}
	}
#endif

	const UToroSettings* Settings = UToroSettings::Get();
	for (const TPair<FString, FString>& Command : Settings->StartupCommands)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(this,
			*FString::Printf(TEXT("%s %s"), *Command.Key, *Command.Value));
	}
}
