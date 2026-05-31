// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "WorldActions/Native/MiscActions.h"
#include "Libraries/ToroWorldLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ToroRuntimeSubsystem.h"
#include "LevelSequencePlayer.h"
#include "Helpers/LatentInfo.h"
#include "ToroGlobals.h"

void UWorldAction_LevelStreaming::OnExecute_Implementation()
{
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Level : Load)
	{
		if (Level.Key.IsNull()) continue;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, 
			Level.Key, Level.Value, false, FLatentInfo::Make());
	}
	for (const TSoftObjectPtr<UWorld>& Level : Unload)
	{
		if (Level.IsNull()) continue;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this,
			Level, FLatentInfo::Make(), false);
	}
}

void UWorldAction_LevelSequence::OnExecute_Implementation()
{
	ALevelSequenceActor* ActorPtr = Sequence.LoadSynchronous();
	// if (ALevelCutsceneActor* Cutscene = Cast<ALevelCutsceneActor>(ActorPtr))
	// {
	// 	Cutscene->PlayCutscene(); // TODO
	// }
	// else 
		if (ULevelSequencePlayer* SequencePlayer = ActorPtr ? ActorPtr->GetSequencePlayer() : nullptr)
	{
		SequencePlayer->RewindForReplay();
		SequencePlayer->Play();
	}
}

void UWorldAction_RemoteEvent::OnExecute_Implementation()
{
	UToroWorldLibrary::CallRemoteEvent(this, EventName);
}

void UWorldAction_GlobalEvent::OnExecute_Implementation()
{
	if (ToroGameplayTags::Event::IsValid(Identifier))
	{
		AActor* InstigatorPtr = Instigator.Get();
		if (!InstigatorPtr)
		{
			InstigatorPtr = Cast<AActor>(const_cast<UObject*>(WorldContext.Get()));
		}

		if (UToroRuntimeSubsystem* Subsystem = UToroRuntimeSubsystem::Get(this))
		{
			Subsystem->InvokeGlobalEvent(Identifier, Payload, InstigatorPtr);
		}
	}
}