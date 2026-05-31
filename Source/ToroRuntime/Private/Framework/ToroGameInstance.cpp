// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroGameInstance.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/FileManager.h"
#include "Misc/App.h"

#define ALLOW_SHIPPING_FEATURES !WITH_EDITOR

#if ALLOW_SHIPPING_FEATURES
#include "ShaderPipelineCache.h"
#include "Libraries/ToroWindowsLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#endif


void UToroGameInstance::RestartGame()
{
	ClearInstanceLock();
#if ALLOW_SHIPPING_FEATURES
	const FProcHandle NewInstHandle = FWindowsPlatformProcess::CreateProc(
		FWindowsPlatformProcess::ExecutablePath(), nullptr, 
		true, false, false, nullptr, 0, nullptr, nullptr
	);

	if (!NewInstHandle.IsValid())
	{
		UToroWindowsLibrary::OpenDialogue(TEXT("Failed to Restart"), 
			TEXT("Game failed to automatically restart, please do so manually."),
			EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
	}

	FPlatformApplicationMisc::RequestMinimize();
#endif
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UToroGameInstance::ClearInstanceLock()
{
	if (InstanceLock)
	{
		InstanceLock->Close();
		delete InstanceLock;
		InstanceLock = nullptr;
	}
	if (!LockPath.IsEmpty())
	{
		IFileManager::Get().Delete(*LockPath, false, false, true);
		LockPath = FString();
	}
}

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	if (InWorld) WorldBeginPlay();
}

void UToroGameInstance::OnWorldTick(UWorld* InWorld, const float DeltaTime)
{
	if (InWorld) WorldTick(DeltaTime);
}

void UToroGameInstance::Init()
{
#if ALLOW_SHIPPING_FEATURES
	FShaderPipelineCache::PauseBatching(); // Pause Shader Compile for instance check

	LockPath = FPaths::ProjectSavedDir() / FApp::GetProjectName() + TEXT(".Lock");
	if (InstanceLock = IFileManager::Get().CreateFileWriter(*LockPath, 0); !InstanceLock)
	{		
		UToroWindowsLibrary::OpenDialogue(TEXT("Failed to start ") + FString(FApp::GetProjectName()), 
			TEXT("Another instance of the game is already running, close that instance first to run. ")
			TEXT("Running multiple instances at the same time may lead to save corruption and other side effects."),
			EWindowsDialogueType::Ok, EWindowsDialogueIcon::Warning);

		FPlatformApplicationMisc::RequestMinimize();
		FPlatformMisc::RequestExit(false);
		return;
	}

	FShaderPipelineCache::ResumeBatching(); // Resume Shader Compile if check passed
#endif

	Super::Init();

	if (UToroGameUserSettings* UserSettings = UToroGameUserSettings::Get())
	{
		UserSettings->InitializeSettings();
	}
}

void UToroGameInstance::Shutdown()
{
	ClearInstanceLock();
	Super::Shutdown();
}
