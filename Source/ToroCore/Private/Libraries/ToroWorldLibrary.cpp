// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroWorldLibrary.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"

UWorld* UToroWorldLibrary::GetPossibleWorld(const UObject* Context)
{
	return FWorldGetter::Get(Context);
}

void UToroWorldLibrary::ReloadLevel(const UObject* ContextObject, const bool bAbsolute, const FString& Options)
{
	const UWorld* World = FWorldGetter::Get(ContextObject);
	UGameplayStatics::OpenLevel(World, *UGameplayStatics::GetCurrentLevelName(World), bAbsolute, Options);
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

EToroLevelStreamState UToroWorldLibrary::GetLevelStreamState(const UObject* ContextObject, const TSoftObjectPtr<UWorld>& Level)
{
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	const ULevelStreaming* StreamedLevel = UGameplayStatics::GetStreamingLevel(ContextObject, LevelName);
	if (!StreamedLevel || !StreamedLevel->ShouldBeLoaded())
	{
		return EToroLevelStreamState::Unloaded;
	}

	return StreamedLevel->ShouldBeVisible() ? EToroLevelStreamState::Visible : EToroLevelStreamState::Loaded;
}

FVoidCoroutine UToroWorldLibrary::SetLevelStreamState(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	ULevelStreaming*& StreamedLevel, const TSoftObjectPtr<UWorld>& Level, const EToroLevelStreamState State)
{
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	StreamedLevel = UGameplayStatics::GetStreamingLevel(ContextObject, LevelName);
	if (!StreamedLevel)
	{
		co_return;
	}

	if (State == EToroLevelStreamState::Unloaded)
	{
		co_await UE5Coro::Latent::ChainEx(&UGameplayStatics::UnloadStreamLevel,
			ContextObject, LevelName, std::placeholders::_2, false);
	}
	else // State == LoadOnly or LoadAndShow
	{
		co_await UE5Coro::Latent::ChainEx(&UGameplayStatics::LoadStreamLevel, ContextObject,
			LevelName, State == EToroLevelStreamState::Visible, false, std::placeholders::_2);
	}

	co_return;
}
