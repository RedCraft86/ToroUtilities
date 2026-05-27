// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroWorldLibrary.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"

UWorld* UToroWorldLibrary::GetPossibleWorld(const UObject* Context)
{
	return FWorldGetter::Get(Context);
}

void UToroWorldLibrary::ReloadLevel(const UObject* ContextObject, const FString Options)
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
