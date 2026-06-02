// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroWorldLibrary.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"
#include "Misc/App.h"
#if WITH_EDITOR
#include "Editor.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

UWorld* UToroWorldLibrary::GetPossibleWorld(const UObject* Context)
{
	return FWorldGetter::Get(Context);
}

void UToroWorldLibrary::ReloadLevel(const UObject* ContextObject, const FString& Options)
{
	const UWorld* World = FWorldGetter::Get(ContextObject);
	UGameplayStatics::OpenLevel(World, *UGameplayStatics::GetCurrentLevelName(World), true, Options);
}

void UToroWorldLibrary::CallRemoteEvent(const UObject* ContextObject, const FName EventName)
{
	if (!EventName.IsNone())
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
		{
			LSA->RemoteEvent(EventName);
		}
	}
}

FTransform UToroWorldLibrary::GetMainCameraTransform(const UObject* ContextObject, const int32 PlayerIdx)
{
	static TFrameValue<FTransform> CameraTransform;
	if (CameraTransform.IsSet())
	{
		return CameraTransform.GetValue();
	}

#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (UUnrealEditorSubsystem* UES = GEditor ? GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>() : nullptr)
		{
			FVector Position;
			FRotator Rotation;
			UES->GetLevelViewportCameraInfo(Position, Rotation);
			CameraTransform = FTransform(Rotation, Position, FVector::OneVector);
		}
	}
	else
#endif
	if (const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(FWorldGetter::Get(ContextObject), PlayerIdx))
	{
		CameraTransform = FTransform(PCM->GetCameraRotation(), PCM->GetCameraLocation(), FVector::OneVector);
	}

	return CameraTransform.GetValue();
}
